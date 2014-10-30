#include "api.h"
#include "battery.h"
#include "adc.h"

#define AVERAGING_WINDOW_SIZE 32

static volatile int16_t measures[AVERAGING_WINDOW_SIZE];
static volatile uint8_t nextPos;

static void battery_task(void);

void Battery_Init(void) {
#ifdef BATTERY_TASK
	uint8_t i;

	os_registerProcessStack(battery_task, "Battery", 200);
	nextPos = 0;
	for (i = 0; i < AVERAGING_WINDOW_SIZE; i++)
		measures[i] = 0;
#endif
}

uint16_t Battery_GetVoltage(void) {
	uint8_t i;
	uint32_t sum = 0;
	for (i = 0; i < AVERAGING_WINDOW_SIZE; i++)
		sum += measures[i];
	return sum / AVERAGING_WINDOW_SIZE;
}

static void battery_task(void) {
	uint16_t batRaw;
	portTickType lastWakeTime = os_getTime();

	for (;;) {
		os_frequency(&lastWakeTime, 50);

		batRaw = ADC_GetValue(10);
		batRaw *= 39;
		batRaw /= 4;
		batRaw += 550;
		measures[nextPos] = batRaw;
		nextPos = (nextPos + 1) % AVERAGING_WINDOW_SIZE;
	}
}
