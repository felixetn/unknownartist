/**
 * (c) Real-time systems project seminar, TU Darmstadt
 *
 * @file adc.c
 * @author Team Highway
 * @brief Provides ADC functionality as used by the line sensor.
 **/

#include "adc.h"
#include "interrupts.h"

static volatile uint16_t adc_values[32];

#define ADC_PINS_MASK ( \
		((uint32_t)ADC_USE_AN00 <<  0) | ((uint32_t)ADC_USE_AN01 <<  1) | ((uint32_t)ADC_USE_AN02 <<  2) | ((uint32_t)ADC_USE_AN03 <<  3) |\
		((uint32_t)ADC_USE_AN04 <<  4) | ((uint32_t)ADC_USE_AN05 <<  5) | ((uint32_t)ADC_USE_AN06 <<  6) | ((uint32_t)ADC_USE_AN07 <<  7) |\
		((uint32_t)ADC_USE_AN08 <<  8) | ((uint32_t)ADC_USE_AN09 <<  9) | ((uint32_t)ADC_USE_AN10 << 10) | ((uint32_t)ADC_USE_AN11 << 11) |\
		((uint32_t)ADC_USE_AN12 << 12) | ((uint32_t)ADC_USE_AN13 << 13) | ((uint32_t)ADC_USE_AN14 << 14) | ((uint32_t)ADC_USE_AN15 << 15) |\
		((uint32_t)ADC_USE_AN16 << 16) | ((uint32_t)ADC_USE_AN17 << 17) | ((uint32_t)ADC_USE_AN18 << 18) | ((uint32_t)ADC_USE_AN19 << 19) |\
		((uint32_t)ADC_USE_AN20 << 20) | ((uint32_t)ADC_USE_AN21 << 21) | ((uint32_t)ADC_USE_AN22 << 22) | ((uint32_t)ADC_USE_AN23 << 23) )

static void init_dma(void);
static void adc_dma_restart(void);
__interrupt static void irq_adc(void);

void ADC_Init(void) {
	uint8_t i;

	ADCS = 0;
	ADCS_BUSY = 1; // ADC is busy
	ADCS_INTE = 1; // Interrupt enabled
	ADCS_MD = 0; // Single mode
	ADCS_S10 = 0; // 8 bit

	ADER0 = (ADC_PINS_MASK >> 0) & 0xff;
	ADER1 = (ADC_PINS_MASK >> 8) & 0xff;
	ADER2 = (ADC_PINS_MASK >> 16) & 0xff;
	ADER3 = 0;

	init_dma();
	interrupts_setHandler(76, 6, irq_adc);

	for (i = 0; i < 32; i++)
		adc_values[i] = 0;
	adc_dma_restart();
}

int16_t ADC_GetValue(uint8_t pin) {
	if ((1 << pin) & ADC_PINS_MASK) {
		return adc_values[pin];
	}
	return -1;
}

static void init_dma(void) {
	DISEL0 = 76; // ADC interrupt number
	IOAH0 = (unsigned char) &ADCR >> 8; // I/O Bank 00
	IOAL0 = (unsigned char) &ADCR & 0xFF;

	DMACS0_BPD = 0; // BAP increment
	DMACS0_IF = 1; // IOA address fixed
	DMACS0_BW = 1; // Word transfers
	DMACS0_BF = 0; // BAP address NOT fixed
	DMACS0_DIR = 0; // Direction: IOA -> BAP
	DMACS0_SE = 0; // No DMA stop request by peripheral

	DER_EN0 = 0; // DMA 0 enable
}

#pragma inline adc_dma_restart
static void adc_dma_restart(void) {
	ADSR = /*ST = 24*CLKP1 */(4 << 13) + /*CT = 33*CLKP1 */(1 << 10) + /*Channel 0*/(0 << 5) + /*Channel 23*/(23 << 0);

	DER_EN0 = 1;
	DCT0 = 48; // Transfer 24*2 Bytes
	DSR_DTE0 = 0; // Clear transfer end interrupt, if any
	BAPH0 = (unsigned long) (adc_values) >> 16;
	BAPM0 = (unsigned long) (adc_values) >> 8;
	BAPL0 = (unsigned long) (adc_values);
	ADCS_STRT = 1;
}

__interrupt static void irq_adc(void) {
	ADCS_INT = 0; // Clear ADC interrupt
	adc_dma_restart();
}
