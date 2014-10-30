/* Demo application includes. */
#include "bluetooth.h"

#include "../../interrupts.h"

/* Scheduler includes. */
#include "queue.h"
#include "task.h"
#include "../../defines.h"

#define BT_BAUD 57600
#define BT_QUEUE_SIZE 256

#define TM_STARTBYTE 0x02
#define TM_STOPBYTE 0x03

/* The queue used to hold received characters. */
static xQueueHandle btxRxedChars;

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
} sendSM;

static uint8_t messagesLeftToSend;
static xQueueHandle sendQueue;

#ifdef BLUETOOTH_TASK
__interrupt static void UART7_RxISR(void);
__interrupt static void UART7_TxISR(void);
#endif

static int16_t Bluetooth_Layer1_GetChar_Async(int blockTimeInMs);
static void Bluetooth_Layer3_ReceivePacket(void);

static int16_t Bluetooth_Layer1_GetChar_Async(int blockTimeInMs) {
	char pcRx = 0;
	//Try to read until the next character is present
	if (xQueueReceive(btxRxedChars, &pcRx, blockTimeInMs / portTICK_RATE_MS)) {
		return pcRx;
	}
	return -1;
}

void Bluetooth_Init(void) {
	// Initialise the hardware.
	portENTER_CRITICAL();
	{
#ifdef BLUETOOTH_TASK
		interrupts_setHandler(89, 6, UART7_RxISR);
		interrupts_setHandler(90, 6, UART7_TxISR);
#endif

// Create the queues used by the com test task.
		btxRxedChars = xQueueCreate(BT_QUEUE_SIZE,
				(unsigned portBASE_TYPE ) sizeof(signed portCHAR));

		DDR00_D0 = 1;
		PDR00_P0 = 0;

		PIER00_IE2 = 1; /* enable input */
		DDR00_D2 = 0; /* switch P00_2 to input */
		DDR00_D1 = 1; /* switch P00_1 to output */

		// Enable relocated Pins
		PRRR8_SOT7_R = 1;
		PRRR8_SIN7_R = 1;

		// Initialize UART asynchronous mode
		BGR7 = configCLKP1_CLOCK_HZ / BT_BAUD;

		SMR7 = 0x09; /* enable SOT3, normal mode */
		SSR7 = 0x02; /* LSB first, enable receive interrupts */
		SMR7 = 0x0d; /* enable SOT3, Reset, normal mode */
		SCR7 = 0x04; /* Clear reception errors */
		__wait_nop();
		__wait_nop();
		SCR7 = 0x13; /* 8N1 */

		messagesLeftToSend = 0;
		sendQueue = xQueueCreate(SENDBUFFER_SIZE, sizeof(wirelessMessage_t));
	}
	portEXIT_CRITICAL();

	sendSM.state = SEND_IDLE;
	os_registerProcessStackPriority(Bluetooth_Layer3_ReceivePacket, "BTLayer3_Receive", 1400, tskIDLE_PRIORITY + 5);
}

/**
 * @brief called by interrupt so send next message
 */
static void getNextMessageToSend(wirelessMessage_t *target) {
	portBASE_TYPE pxTaskWoken = pdFALSE;

	if (messagesLeftToSend > 0) {
		xQueueReceiveFromISR(sendQueue, target, &pxTaskWoken);
		messagesLeftToSend--;
	}
}

static uint8_t* putStuffedByte(uint8_t* pos, uint8_t byte) {
	if (byte == TM_STARTBYTE || byte == TM_STOPBYTE
	)
		*pos++ = byte;
	*pos++ = byte;
	return pos;
}

static void startSendingNextMessage(void) {
	if (sendSM.state == SEND_IDLE) {
		if (messagesLeftToSend > 0) {
			uint8_t i = 0;
			uint8_t stuffed_len = 0;
			uint8_t* bufferPos;
			static wirelessMessage_t message;

			sendSM.state = SEND_SENDING;

			getNextMessageToSend(&message);

			for (i = 0; i < message.dataLength + 5; i++) {
				stuffed_len++;
				if (((uint8_t*) &message)[i] == TM_STARTBYTE || ((uint8_t*) &message)[i] == TM_STOPBYTE) {
					stuffed_len++;
				}
			}

			sendSM.commandString[0] = TM_STARTBYTE;
			sendSM.commandString[1] = 0x80 | stuffed_len;
			bufferPos = (uint8_t*) &sendSM.commandString[2];
			bufferPos = putStuffedByte(bufferPos, message.sourceId);
			bufferPos = putStuffedByte(bufferPos, message.destinationId);
			bufferPos = putStuffedByte(bufferPos, message.messageType);
			bufferPos = putStuffedByte(bufferPos, message.priority);
			bufferPos = putStuffedByte(bufferPos, message.dataLength);
			for (i = 0; i < message.dataLength; i++) {
				bufferPos = putStuffedByte(bufferPos, message.data[i]);
			}
			*bufferPos++ = TM_STOPBYTE;

			sendSM.commandLength = bufferPos - (uint8_t*) sendSM.commandString;

			sendSM.commandSendPos = 0;
			TDR7 = sendSM.commandString[0];
			SSR7_TIE = 1;
		}
	}
}

/*-----------------------------------------------------------*/

/*-----------------------------------------------------------*/

#ifdef BLUETOOTH_TASK
/*
 * UART RX interrupt service routine.
 */
__interrupt static void UART7_RxISR(void) {
	volatile signed portCHAR cChar;
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

	/* Get the character from the UART and post it on the queue of Rxed
	 characters. */
	cChar = RDR7;

	xQueueSendFromISR( btxRxedChars, ( const void *const ) &cChar, &xHigherPriorityTaskWoken);

	if (xHigherPriorityTaskWoken) {
		/*If the post causes a task to wake force a context switch
		 as the woken task may have a higher priority than the task we have
		 interrupted. */
		portYIELD_FROM_ISR();
	}
}
/*-----------------------------------------------------------*/

/*
 * UART Tx interrupt service routine.
 */
__interrupt static void UART7_TxISR(void) {
	sendSM.commandSendPos++;
	if (sendSM.commandSendPos < sendSM.commandLength) {
		/* There was another character queued - transmit it now. */
		TDR7 = sendSM.commandString[sendSM.commandSendPos];
	} else {
		/* Disable transmit interrupts */
		SSR7_TIE = 0;
		sendSM.state = SEND_IDLE;
	}
}
#endif

/*
 * Receive layer3-packet bytewise and send it to layer4
 */
static void Bluetooth_Layer3_ReceivePacket(void) {
	enum states {
		IDLE, READ_DATALENGTH, IN_RECEIVING
	};

	uint8_t byteOk = 0;
	wirelessMessage_t message;
	int16_t actual_char = -1;
	uint8_t last_char = 0; // characters read
	uint8_t state = IDLE; //initial state
	uint8_t data_length = 0; //payload length including byte stuffing
	uint8_t pos = 0; //position in user-data
	uint8_t received_bytes = 0; //number of bytes received

	for (;;) {
		last_char = actual_char;

		do {
			actual_char = Bluetooth_Layer1_GetChar_Async(10);
		} while (actual_char < 0);
		byteOk = 1;

		switch (state) {
			case IDLE:
				//start if STX-byte is found
				if (actual_char == TM_STARTBYTE) {
					state = READ_DATALENGTH;
					received_bytes = 0;
				}
				break;

			case READ_DATALENGTH:
				data_length = actual_char & 0x7f;
				received_bytes = 0;
				pos = 0;
				state = IN_RECEIVING;
				break;

			case IN_RECEIVING:
				if (++received_bytes <= data_length + 1) {
					switch (actual_char) {
						case TM_STOPBYTE:
							if (received_bytes == data_length + 1) {
								//end of the frame reached
								wirelessReceive(WI_IF_BLUETOOTH, &message);
								state = IDLE;
								byteOk = 0;
							} else if (last_char != TM_STOPBYTE) { //remove bytestuffed ETX
								byteOk = 0;
							}
							break;

						case TM_STARTBYTE:
							if (last_char != TM_STARTBYTE) { //remove bytestuffed STX
								byteOk = 0;
							}
							break;
					}
					if (byteOk) {
						switch (pos) {
							case 0:
								message.sourceId = actual_char;
								break;
							case 1:
								message.destinationId = actual_char;
								break;
							case 2:
								message.messageType = actual_char;
								break;
							case 3:
								message.priority = actual_char;
								break;
							case 4:
								message.dataLength = actual_char;
								break;
							default:
								message.data[pos - 5] = actual_char;
						}
						pos++;
					}
				} else {
					state = IDLE;
				}
				break;

			default:
				state = IDLE;
				break;
		}
	}
}

void Bluetooth_Send(wirelessMessage_t* msg) {
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	portBASE_TYPE returnpd = pdFALSE;

	returnpd = xQueueSendFromISR( sendQueue, msg,
			&xHigherPriorityTaskWoken);

	if (returnpd == pdTRUE) {
		messagesLeftToSend++;
		startSendingNextMessage();
	}
}
