/*
 * api.c
 *
 *  Created on: 28.06.2011
 *      Author: Christian Illy
 */
#include "api.h"
#include "carid.h"

uint8_t carid = 0;

void CarId_Init(void) {
	DDR02 &= ~0x3c;
	PIER02 |= 0x3c;
	__wait_nop();
	carid = ((~PDR02 & 0x3c) >> 2) + 1;
}
