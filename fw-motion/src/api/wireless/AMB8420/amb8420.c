#include "amb8420.h"
#include "../../interrupts.h"
#include <stdlib.h>

/* Scheduler includes. */
#include "queue.h"
#include "task.h"
#include <stdio.h>
#include <string.h>
#include "../../util.h"
#include "../../api.h"
#include "../../defines.h"
#include "../../carid.h"

#define W_BAUD 115200
#define W_QUEUE_SIZE 192

#define WIRELESS_CHANNEL 0x6A
#define WIRELESS_OP_DATA_LENGTH 16

// Register number for channel
#define REG_CHANNEL 42
// Register number for source Id
#define REG_SOURCEID 29

static xTaskHandle ambTask;
static xTaskHandle ambInitTask;

static uint8_t messagesLeftToSend;
static xQueueHandle sendQueuePrio1;
static xQueueHandle sendQueuePrio2;
static xQueueHandle sendQueuePrio3;

// State machine struct for receiving
static struct {
	enum {
		REC_IDLE,
		REC_OPCODE,
		REC_OP_MESSAGELENGTH,
		REC_OP_DATA,
		REC_DATA_MESSAGELENGTH,
		REC_SOURCE_ID,
		REC_DEST_ID,
		REC_MESSAGE_TYPE,
		REC_DATA,
		REC_PRIORITY,
		REC_FELDSTAERKE,
		REC_CS
	} state;

	// The queue used to hold received characters.
	xQueueHandle wxRxedChars;
} receiveSM;

// State machine struct for sending
static struct {
	enum {
		SEND_IDLE, // Ready for a new command to send
		SEND_SENDING
	// Currently sending a command
	} state;

	// Current sending array pos
	uint8_t commandSendPos;

	// Current commands length
	uint8_t commandLength;

	// Command string that is currently sent
	char commandString[UARTMESSAGE_BUFFER_SIZE];

	// Command in buffer was sent out and has received a success reply
	boolean_t commandDone;

	// If the last command was a read register command and we received a reply it is stored here
	int16_t readReply;
} sendSM;

/*
 * Global and local functions
 */
static uint8_t readCommand(uint8_t regNum);
static void writeCommand(uint8_t regNum, uint8_t val);
static void AMB8420_CS_calc(void);
static int16_t AMB8420_GetChar_Async(int blockTimeInMs);
static void AMB8420_ReceiveMessage(void);
static void sendCommandBuffer(void);
static void AMB8420_startSendingNextMessage(void);

__interrupt static void UART8_RxISR(void);
__interrupt static void UART8_TxISR(void);

static void AMB8420_config(void) {
	uint8_t src = readCommand(REG_SOURCEID);
	uint8_t chn = readCommand(REG_CHANNEL);
	uint8_t src2, chn2;
	if (src != carid)
		writeCommand(REG_SOURCEID, carid);
	if (chn != WIRELESS_CHANNEL
	)
		writeCommand(REG_CHANNEL, WIRELESS_CHANNEL);
	src2 = readCommand(REG_SOURCEID);
	chn2 = readCommand(REG_CHANNEL);
	wirelessFormattedDebugMessage(WI_IF_AMB8420, "Src: %d, Chn: %d;  CarID: %d;  Src: %d, Chn: %d", src, chn, carid,
			src2, chn2);
	wirelessDebugMessage(WI_IF_AMB8420, "Boot sequence complete");
	ambInitTask = 0;
	os_deleteTask(0);
	for (;;)
		os_wait(1000);
}

/*
 * @brief: Initialisation the connection to the AMB8420 Board
 *
 */
boolean_t AMB8420_Init(void) {
#ifdef AMB8420_TASK
	// Initialise the hardware.
	ambTask = 0;
	ambInitTask = 0;
	portENTER_CRITICAL();
	{
		interrupts_setHandler(91, 6, UART8_RxISR);
		interrupts_setHandler(92, 6, UART8_TxISR);

		// Create the queues used by the com test task.
		receiveSM.wxRxedChars = xQueueCreate(UARTMESSAGE_BUFFER_SIZE,
				(unsigned portBASE_TYPE ) sizeof(signed portCHAR));

		//RTS connected to P00_3 as Input
		PIER00_IE3 = 1; // enable input
		DDR00_D3 = 0;

		//Reset connected to P00_6 as Output
		PDR00_P6 = 1;
		DDR00_D6 = 1;

		// Pins of UART8:
		PIER00_IE5 = 1; // enable input
		DDR00_D5 = 0; // switch P00_5 to input
		DDR00_D4 = 1; // switch P00_4 to output

		// Enable relocated Pin functions
		PRRR8_SIN8_R = 1;
		PRRR8_SOT8_R = 1;

		// Initialize UART asynchronous mode
		BGR8 = configCLKP1_CLOCK_HZ / W_BAUD;
		SSR8 = 0x02; /* LSB first, enable receive interrupts */
		SMR8 = 0x0d; /* enable SOT3, Reset, normal mode */
		SCR8 = 0x17; /* 8N1 */
	}
	portEXIT_CRITICAL();

	messagesLeftToSend = 0;
	sendQueuePrio1 = xQueueCreate(SENDBUFFER_SIZE, sizeof(wirelessMessage_t));
	sendQueuePrio2 = xQueueCreate(SENDBUFFER_SIZE, sizeof(wirelessMessage_t));
	sendQueuePrio3 = xQueueCreate(SENDBUFFER_SIZE, sizeof(wirelessMessage_t));

	/*register the AMB8420_ReceiveMessage function as a task*/
	ambTask = os_registerProcessStackPriority(AMB8420_ReceiveMessage, "AMB8420 receive", 1800, tskIDLE_PRIORITY + 5);
	ambInitTask = os_registerProcessStack(AMB8420_config, "AMB8420 config", 300);

	sendSM.commandDone = TRUE;
	sendSM.state = SEND_IDLE;
	receiveSM.state = REC_IDLE;
	return TRUE;
#else
	return FALSE;
#endif
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//
// TRANSMIT PART
//
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

static void sendCommandBuffer(void) {
	sendSM.state = SEND_SENDING;
	sendSM.commandDone = FALSE;
	sendSM.commandSendPos = 0;
	TDR8 = sendSM.commandString[0];
	SSR8_TIE = 1;
}

/*
 * Calculates the CS for the current command string
 */
static void AMB8420_CS_calc(void) {
	uint8_t i = 0;
	char checkSum = 0;

	for (i = 0; i < (sendSM.commandLength - 1); i++)
		checkSum = checkSum ^ sendSM.commandString[i];

	sendSM.commandString[sendSM.commandLength - 1] = checkSum;
}

/**
 * @brief called by interrupt so send next message
 */
static void getNextMessageToSend(wirelessMessage_t *target) {
	static prioCounter = 1;
	uint8_t found = 0;

	portBASE_TYPE pxTaskWoken = pdFALSE;

	if (messagesLeftToSend > 0) {
		if (prioCounter > PRIORITY3
		)
			prioCounter = 1;

		// Bei jeder neunten Nachricht wird eine Nachricht der Priorität 3 gesendet (kann nach Belieben verändert werden)
		if (((prioCounter % PRIORITY3) == 0) && (uxQueueMessagesWaiting(sendQueuePrio3) != 0)) {
			xQueueReceiveFromISR(sendQueuePrio3, target, &pxTaskWoken);
			found = 1;
		} else if (((prioCounter % PRIORITY2) == 0) && (uxQueueMessagesWaiting(sendQueuePrio2) != 0)) {
			// Bei jeder dritten Nachricht wird eine Nachricht der Priorität 2 gesendet
			xQueueReceiveFromISR(sendQueuePrio2, target, &pxTaskWoken);
			found = 1;
		} else if (uxQueueMessagesWaiting(sendQueuePrio1) != 0) {
			// ansonsten erst die prio 1
			xQueueReceiveFromISR(sendQueuePrio1, target, &pxTaskWoken);
			found = 1;
		} else if (uxQueueMessagesWaiting(sendQueuePrio2) != 0) {
			// ansonsten prio 2
			xQueueReceiveFromISR(sendQueuePrio2, target, &pxTaskWoken);
			found = 1;
		} else if (uxQueueMessagesWaiting(sendQueuePrio3) != 0) {
			// ansonsten prio 3
			xQueueReceiveFromISR(sendQueuePrio3, target, &pxTaskWoken);
			found = 1;
		}

		if (found != 0) {
			prioCounter++;
			messagesLeftToSend--;
		}
	}
}

/**
 *	@brief: This function is for start sending a message manually
 *			This is needed if a new message was added to the empty sendqueue.
 */
static void AMB8420_startSendingNextMessage() {
	if (ambInitTask != 0)
		return;

	if ((receiveSM.state == REC_IDLE) && (sendSM.state == SEND_IDLE)) {
		if ((messagesLeftToSend > 0) && (sendSM.commandDone == TRUE)) {
			uint8_t i = 0;
			uint8_t msgLength;
			static wirelessMessage_t message;

			sendSM.state = SEND_SENDING;

			getNextMessageToSend(&message);

			msgLength = message.dataLength + 3 + 2;

			sendSM.commandString[0] = 0x02; //Init-Byte
			sendSM.commandString[1] = 0x01; //Kommand-Byte: Sende Nachricht im Adressierungsmodus 2
			sendSM.commandString[2] = msgLength; // Anzahl der Datenbytes + wirelessLayer0_message-overhead + 3-Byte Kommand-Overhead
			sendSM.commandString[3] = WIRELESS_CHANNEL;
			sendSM.commandString[4] = message.destinationId; // Parameter um den Empfänger der Nachricht im Funkmodul konfigurieren
			sendSM.commandString[5] = message.destinationId; // Adresse wird nochmal in den Nutzdaten gesendet
			sendSM.commandString[6] = message.messageType;
			sendSM.commandString[7] = message.priority;
			for (i = 0; i < message.dataLength; i++) {
				sendSM.commandString[8 + i] = message.data[i];
			}

			sendSM.commandLength = msgLength + 4;

			AMB8420_CS_calc();
			sendCommandBuffer();
		} else if (sendSM.commandDone == FALSE) {
			sendCommandBuffer();
		}
	}
}

void AMB8420_Send(wirelessMessage_t* msg) {
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	portBASE_TYPE returnpd = pdFALSE;

	switch (msg->priority) {
		case 1:
			returnpd = xQueueSendFromISR( sendQueuePrio1, msg,
					&xHigherPriorityTaskWoken);
			break;

		case 3:
			returnpd = xQueueSendFromISR( sendQueuePrio3, msg,
					&xHigherPriorityTaskWoken);
			break;

		default:
			returnpd = xQueueSendFromISR( sendQueuePrio2, msg,
					&xHigherPriorityTaskWoken);
			break;
	}

	if (returnpd == pdTRUE) {
		messagesLeftToSend++;
		AMB8420_startSendingNextMessage();
	}
}

/*
 *
 * UART Tx interrupt service routine.

 * The Routine has to be configured in vectors.c:
 * 		1. declare the Routine as an interrupt routine
 * 		2. set  #pragma intvect UART8_TxISR 92   // UART 8 RX
 *		3. define at  InitIrqLevels(void) the Interruptlevel of the Service -->  ICR = (92 << 8) | 6;	// UART 8 Rx
 */
#ifdef AMB8420_TASK
__interrupt static void UART8_TxISR(void) {
	sendSM.commandSendPos++;
	if (sendSM.commandSendPos < sendSM.commandLength) {
		/* There was another character queued - transmit it now. */
		TDR8 = sendSM.commandString[sendSM.commandSendPos];
	} else {
		/* Disable transmit interrupts */
		SSR8_TIE = 0;
		sendSM.state = SEND_IDLE;
	}
}
#endif

static void writeCommand(uint8_t regNum, uint8_t val) {
	sendSM.commandString[0] = 0x02;
	sendSM.commandString[1] = 0x09;
	sendSM.commandString[2] = 0x03;
	sendSM.commandString[3] = regNum;
	sendSM.commandString[4] = 1;
	sendSM.commandString[5] = val;
	sendSM.commandLength = 7;
	AMB8420_CS_calc();
	sendCommandBuffer();
	while (sendSM.commandDone != TRUE)
		os_wait(2);
}

static uint8_t readCommand(uint8_t regNum) {
	sendSM.commandString[0] = 0x02;
	sendSM.commandString[1] = 0x0A;
	sendSM.commandString[2] = 0x02;
	sendSM.commandString[3] = regNum;
	sendSM.commandString[4] = 1;
	sendSM.commandLength = 6;
	AMB8420_CS_calc();
	sendCommandBuffer();
	sendSM.readReply = -1;
	while (sendSM.readReply < 0) {
		os_wait(2);
	}
	return sendSM.readReply;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//
// RECEIVE PART
//
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

static int16_t AMB8420_GetChar_Async(int blockTimeInMs) {
	char pcRx = 0;
	/* Get the next character from the buffer.  Return false if no characters
	 are available, or arrive before xBlockTime expires. */
	if (xQueueReceive(receiveSM.wxRxedChars, (signed char*) (&pcRx), blockTimeInMs / portTICK_RATE_MS)) {
		return pcRx;
	}
	return -1;
}

static void replyReceived(uint8_t* data) {
	uint8_t i;

	switch (data[1]) {
		case 0x4a:
			sendSM.readReply = data[5];
			break;

		case 0x40:
			if (data[3] < 0x02) {
				// correct message command
			}
			break;

#ifdef RF_DEBUG
		default:
			printf("OP_DATA: return: ");
			for (i = 0; i < data[2] + 4; i++) {
				printf("%x  ", data[i]);
			}
			printf("\r\n");
			break;
#endif
	}
	sendSM.commandDone = TRUE;
}

/**
 * @brief: receive Data. Function seperates different dataStrings and commandFeadbacks (return strings)
 * every step 1 Byte is received.
 *
 * This function is a registered Task with a priority defined in AMB8420_init()
 */
static void AMB8420_ReceiveMessage() {
	wirelessMessage_t message;
	static char returnedState[WIRELESS_OP_DATA_LENGTH];

	int16_t receivedChar;
	uint8_t currentChar;
	uint8_t dataCount = 0;
	uint8_t i = 0; //counter für Schleifen
	for (;;) {
		do {
			receivedChar = AMB8420_GetChar_Async(10);
			if (receivedChar < 0)
				AMB8420_startSendingNextMessage();
		} while (receivedChar < 0);

		currentChar = (uint8_t) (receivedChar & 0x00ff);

		switch (receiveSM.state) {

			case REC_IDLE:
				if (currentChar == 0x02) //startByte
						{
					dataCount = 0;
					receiveSM.state = REC_OPCODE;
				}
				break;

				/*checkt the OPCODE to decide which message is received: either a dataMessage(actualChar == 0x81) or a systemMessage which is returned from the module */
			case REC_OPCODE:
				if (currentChar == 0x81) {
					receiveSM.state = REC_DATA_MESSAGELENGTH;
					sendSM.state = SEND_IDLE;
				} else {
					returnedState[0] = 0x02;
					returnedState[1] = currentChar;
					receiveSM.state = REC_OP_MESSAGELENGTH;
				}
				break;

			case REC_OP_MESSAGELENGTH:
				if (currentChar + 4 > WIRELESS_OP_DATA_LENGTH) {
#ifdef RF_DEBUG
					printf("Fehler beim Auswerten von OP-Daten!! \n\r");
#endif
					receiveSM.state = REC_CS;
				} else {
					returnedState[2] = currentChar;
					if (currentChar > 0)
						receiveSM.state = REC_OP_DATA;
					else
						receiveSM.state = REC_CS;
				}
				break;

			case REC_OP_DATA:
				returnedState[dataCount + 3] = currentChar;
				dataCount++;
				if (dataCount >= returnedState[2]) {
					replyReceived((uint8_t*) returnedState);
					receiveSM.state = REC_CS;
				}
				break;

			case REC_DATA_MESSAGELENGTH:
				message.dataLength = currentChar - 5; //Empfangene Daten - 7 Overhead der Layer0message und -3 overhead vom Return des Funkmoduls
				receiveSM.state = REC_SOURCE_ID;
				break;

			case REC_SOURCE_ID:
				message.sourceId = currentChar;
				receiveSM.state = REC_DEST_ID;
				break;

			case REC_DEST_ID:
				message.destinationId = currentChar;
				receiveSM.state = REC_MESSAGE_TYPE;
				break;

			case REC_MESSAGE_TYPE:
				message.messageType = currentChar;
				receiveSM.state = REC_PRIORITY;
				break;

			case REC_PRIORITY:
				message.priority = currentChar;
				if (message.dataLength > 0)
					receiveSM.state = REC_DATA;
				else
					receiveSM.state = REC_FELDSTAERKE;
				break;

			case REC_DATA:
				message.data[dataCount] = currentChar;
				dataCount++;
				if (dataCount >= message.dataLength) {
					receiveSM.state = REC_FELDSTAERKE;
					wirelessReceive(WI_IF_AMB8420, &message);
				}
				break;

			case REC_FELDSTAERKE: /*die Feldstärke wird in unseren Ansätzen nicht benötigt und deswegen auch nicht abgespeichert! Steht in diesem State in --> actualChar	*/
				// feldstaerke = actualChar;
				receiveSM.state = REC_CS;
				break;

			case REC_CS:
				// checksum = actualChar;
				receiveSM.state = REC_IDLE;
				AMB8420_startSendingNextMessage();
				break;

			default:
				receiveSM.state = REC_IDLE;
				break;
		}

	}

}

#ifdef AMB8420_TASK
__interrupt
static void UART8_RxISR(void) {
	signed portCHAR cChar;
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

	cChar = RDR8;
	xQueueSendFromISR( receiveSM.wxRxedChars, ( const void *const ) &cChar, &xHigherPriorityTaskWoken);

	if (xHigherPriorityTaskWoken) {
		/*If the post causes a task to wake force a context switch
		 as the woken task may have a higher priority than the task we have
		 interrupted. */
		portYIELD_FROM_ISR();
	}
}
#endif
