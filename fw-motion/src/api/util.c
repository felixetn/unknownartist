/******************************************************************************
 /* (C) Fujitsu Microelectronics Europe GmbH                                  */
/*                                                                           */
/* The following software deliverable is intended for and must only be       */
/* used in an evaluation laboratory environment.                             */
/* It is provided without charge and therefore provided on an as-is          */
/* basis, subject to alterations.                                            */
/*                                                                           */
/* The software deliverable is to be used exclusively in connection          */
/* with FME products.                                                        */
/* In the event the software deliverable includes the use of open            */
/* source components, the provisions of the governing open source            */
/* license agreement shall apply with respect to such software               */
/* deliverable.                                                              */
/* FME does not warrant that the deliverables do not infringe any            */
/* third party intellectual property right (IPR). In the event that          */
/* the deliverables infringe a third party IPR it is the sole                */
/* responsibility of the customer to obtain necessary licenses to            */
/* continue the usage of the deliverable.                                    */
/*                                                                           */
/* To the maximum extent permitted by applicable law FME disclaims all       */
/* warranties, whether express or implied, in particular, but not            */
/* limited to, warranties of merchantability and fitness for a               */
/* particular purpose for which the deliverable is not designated.           */
/*                                                                           */
/* To the maximum extent permitted by applicable law, FME�s liability        */
/* is restricted to intention and gross negligence.                          */
/* FME is not liable for consequential damages.                              */
/*                                                                           */
/* (Disclaimer V1.2)                                                         */
/*****************************************************************************/
/** \file util.c
 **
 ** Utility routines
 **
 ** History:
 **   - 2009-12-17  1.00  CIl  First version
 *****************************************************************************/

/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/
#include "util.h"

/*****************************************************************************/
/* Local pre-processor symbols/macros ('#define')                            */
/*****************************************************************************/

/*****************************************************************************/
/* Global variable definitions (declared in header file with 'extern')       */
/*****************************************************************************/

/*****************************************************************************/
/* Local type definitions ('typedef')                                        */
/*****************************************************************************/

/*****************************************************************************/
/* Local variable definitions ('static')                                     */
/*****************************************************************************/

/*****************************************************************************/
/* Local function prototypes ('static')                                      */
/*****************************************************************************/

/*****************************************************************************/
/* Function implementation - global ('extern') and local ('static')          */
/*****************************************************************************/

int8_t abs8(int8_t val) {
	if (val < 0)
		return -val;
	return val;
}

int16_t abs16(int16_t val){
	if (val < 0) {
		return -val;
	} else {
		return val;
	}
}

int32_t abs32(int32_t value) {
	if (value < 0) {
		return -value;
	} else {
		return value;
	}
}

en_result_t intToStr(uint32_t data, uint8_t digits, char_t fillChar,
		char_t* str) {
	uint8_t i;

	if ((digits < 1) || (digits > 10)) {
		return ErrorInvalidParameter;
	}

	for (i = 0; i < digits; i++) {
		str[digits - i - 1] = '0' + (data % 10);
		data /= 10;
	}

	for (i = 0; i < (digits - 1); i++) {
		if (str[i] == '0')
			str[i] = fillChar;
		else
			break;
	}

	str[digits] = 0;

	return Ok;
}

void wait(uint32_t del) {
	uint32_t i;
	for (i = 0; i < del; i++)
		__wait_nop();
}

void wait1us(uint16_t delay_us) {
	TMRLR3 = delay_us - 1;
	TMCSR3 = 0x0403; // 1us@16MHz, No Reload
	while (TMCSR3_UF == 0)
		;
}

void wait1ms(uint16_t delay_ms) {
	uint16_t i;
	for (i = 0; i < delay_ms; i++)
		wait1us(1000);
}
