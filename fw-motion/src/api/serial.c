/* Standard includes. */
#include <stdlib.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

/* Demo application includes. */
#include "serial.h"
#include "interrupts.h"

/* The queue used to hold received characters. */
static xQueueHandle xRxedChars;

/* The queue used to hold characters waiting transmission. */
static xQueueHandle xCharsForTx;

static volatile portSHORT sTHREEmpty;

__interrupt static void UART0_RxISR(void);
__interrupt static void UART0_TxISR(void);

int16_t Serial_GetcharAsync(int blockTimeInMs) {
#ifndef DEBUGMCU
	char pcRx = 0;
	//Try to read until the next character is present
	if (xQueueReceive(xRxedChars, &pcRx, blockTimeInMs / portTICK_RATE_MS)) {
		return pcRx;
	}
	return -1;
#else
	return -1;
#endif
}

char Serial_Getchar(void) {
	int16_t pcRx;
#ifndef DEBUGMCU
	//Try to read until the next character is present
	while ((pcRx = Serial_GetcharAsync(40 / portTICK_RATE_MS)) < 0)
		;
#endif
	return (char) pcRx;
}

void Serial_Init(void) {
#ifndef DEBUGMCU
	/* Initialise the hardware. */
	portENTER_CRITICAL();
	{
		interrupts_setHandler(79, 6, UART0_RxISR);
		interrupts_setHandler(80, 6, UART0_TxISR);

		/* Create the queues used by the com test task. */
		xRxedChars = xQueueCreate( 128, ( unsigned portBASE_TYPE ) sizeof(signed portCHAR) );
		xCharsForTx = xQueueCreate( 128, ( unsigned portBASE_TYPE ) sizeof(signed portCHAR) );

		/* Initialize UART asynchronous mode */BGR0 = configCLKP1_CLOCK_HZ / 115200;

		SCR0 = 0x17; /* 8N1 */
		SMR0 = 0x0d; /* enable SOT3, Reset, normal mode */
		SSR0 = 0x02; /* LSB first, enable receive interrupts */

		PIER08_IE2 = 1; /* enable input */
		DDR08_D2 = 0; /* switch P08_2 to input */
		DDR08_D3 = 1; /* switch P08_3 to output */
	}
	portEXIT_CRITICAL();
#endif
}

/*-----------------------------------------------------------*/
void Serial_PutChar(char Char) {
#ifndef DEBUGMCU
	/* Transmit a character. */
	portENTER_CRITICAL();
	{
		if (sTHREEmpty == pdTRUE) {
			/* If sTHREEmpty is true then the UART Tx ISR has indicated that
			 there are no characters queued to be transmitted - so we can
			 write the character directly to the shift Tx register. */
			sTHREEmpty = pdFALSE;
			TDR0 = Char;
			SSR0_TIE = 1;
		} else {
			/* sTHREEmpty is false, so there are still characters waiting to be
			 transmitted.  We have to queue this character so it gets
			 transmitted	in turn. */

			/* Return false if after the block time there is no room on the Tx
			 queue.  It is ok to block inside a critical section as each task
			 maintains it's own critical section status. */
			if (xQueueSend(xCharsForTx, &Char, 10/portTICK_RATE_MS) == pdTRUE) {
				SSR0_TIE = 1;
			}
		}
	}
	portEXIT_CRITICAL();
#endif
}

/*-----------------------------------------------------------*/

/*
 * UART RX interrupt service routine.
 */
__interrupt
static void UART0_RxISR(void) {
	volatile signed portCHAR cChar;
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

	/* Get the character from the UART and post it on the queue of Rxed
	 characters. */
	cChar = RDR0;

	xQueueSendFromISR( xRxedChars, ( const void *const ) &cChar, &xHigherPriorityTaskWoken);

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
__interrupt
static void UART0_TxISR(void) {
	signed portCHAR cChar;
	signed portBASE_TYPE xTaskWoken = pdFALSE;

	/* The previous character has been transmitted.  See if there are any
	 further characters waiting transmission. */
	if (xQueueReceiveFromISR(xCharsForTx, &cChar, &xTaskWoken) == pdTRUE) {
		/* There was another character queued - transmit it now. */
		TDR0 = cChar;
	} else {
		/* There were no other characters to transmit. */
		sTHREEmpty = pdTRUE;

		/* Disable transmit interrupts */
		SSR0_TIE = 0;
	}
}
