#include "us.h"
#include "interrupts.h"

static uint8_t rxBuffer2[6];

static uint16_t timeout2 = 0;

static boolean_t cmdReplyWait2 = FALSE;

volatile UsSensorData_t Us_Data;

/*****************************************************************************/
/* Local function prototypes ('static')                                      */
/*****************************************************************************/
static void Us_Systick(void);
static void sendByte(uint8_t module, uint8_t data);
static en_result_t sendCommand(uint8_t module, uint8_t cmd, uint8_t write,
		uint8_t data);
__interrupt void irq_us2_Rx(void);

/*****************************************************************************/
/* Function implementation - global ('extern') and local ('static')          */
/*****************************************************************************/

void Us_Init(void) {
#ifdef US_TASK
	interrupts_setHandler(85, 5, irq_us2_Rx);

	// Channel 2
	DDR01_D3 = 1; // SOT3
	DDR01_D2 = 0; // SIN3
	PIER01_IE2 = 1; // SIN3

	// USART settings
	SCR3 = 0x17;
	SMR3 = 0x01;
	SSR3 = 0x02;
	BGR3 = 15;

	cmdReplyWait2 = FALSE;

	I2CUs_Init();
	os_registerProcessStackPriority(Us_Systick, "US Systick", 120, tskIDLE_PRIORITY + 4);
#endif
}

static void sendByte(uint8_t module, uint8_t data) {
	while (!SSR3_TDRE)
		;
	TDR3 = data;
}

static en_result_t sendCommand(uint8_t module, uint8_t cmd, uint8_t write,
		uint8_t data) {
	uint8_t byte = (cmd & 0x07) | ((write & 0x01) << 3) | ((data & 0x0f) << 4);
	if (module != 2)
		return ErrorInvalidParameter;
	if (module == 2) {
		sendByte(2, byte);
		cmdReplyWait2 = TRUE;
	}
	return Ok;
}

boolean_t Us_Cmd_Wait(uint8_t module) {
	if (module == 2)
		return cmdReplyWait2;
	else
		return FALSE;
}

en_result_t Us_Calibrate(uint8_t module) {
	return sendCommand(module, 0x02, 1, 0x0a);
}

en_result_t Us_IO_DDR(uint8_t module, uint8_t ioNr, uint8_t ddr) {
	if (ioNr > 3)
		return ErrorInvalidParameter;
	if (ddr > 1)
		return ErrorInvalidParameter;
	return sendCommand(module, 0x05, 1, ioNr | (ddr << 2));
}

en_result_t Us_IO_Value(uint8_t module, uint8_t ioNr, uint8_t value) {
	if (ioNr > 3)
		return ErrorInvalidParameter;
	if (value > 1)
		return ErrorInvalidParameter;
	return sendCommand(module, 0x04, 1, ioNr | (value << 2));
}

static void Us_Systick(void) {
	for (;;) {
//		os_frequency(&lastWakeTime, 40); // I2CUs does have waits anyway

		if (timeout2 > 0)
			timeout2--;
		else
			Us_Data.Rear_DataValid = FALSE;

		I2CUs_Systick();
	}
}

#ifdef US_TASK
__interrupt void irq_us2_Rx(void) {
	uint8_t data = RDR3;
	if ((SSR3 & 0xE0) != 0) {
		SCR3_CRE = 1;
		return;
	}
	if (data == 0) {
		rxBuffer2[0] = 0;
	} else {
		rxBuffer2[0]++;
		rxBuffer2[rxBuffer2[0]] = data;

		switch (rxBuffer2[1]) {
			case 0x01:
				// raw data
				break;
			case 0x02:
				// evaluated data
				if (rxBuffer2[0] >= 4) {
					Us_Data.Rear_Distance = ((rxBuffer2[3] & 0x7f) << 7)
									+ (rxBuffer2[4] & 0x7f);
					Us_Data.Rear_DataValid = TRUE;
					timeout2 = 100;
					rxBuffer2[0] = 0;
				}
				break;
			case 0x03:
				// cmd reply
				if (rxBuffer2[0] >= 2) {
					cmdReplyWait2 = FALSE;
					rxBuffer2[0] = 0;
				}
				break;
			case 0x04:
				// extended cmd replies
				if (rxBuffer2[0] >= 2) {
					if (rxBuffer2[2] == 0x10)
						// Calibration done
						break;
					if (rxBuffer2[2] == 0x11)
						// unk cmd
						break;
				}
				break;
		}
		if (rxBuffer2[0] > 4)
			rxBuffer2[0] = 0;
		/*
		 if (rxBuffer2[0] >= 4)
		 {
		 if (rxBuffer2[1] == 0x02)
		 {
		 Us_Distance2Raw = rxBuffer2[2];
		 Us_Distance2Eval = ((rxBuffer2[3] & 0x7f) << 7) + (rxBuffer2[4]
		 & 0x7f);
		 Us_DataValid2 = TRUE;
		 timeout2 = 1000;
		 }
		 rxBuffer2[0] = 0;
		 }*/
	}
}
#endif
