/*
 * api.c
 *
 *  Created on: 28.05.2010
 *      Author: Team Motion
 */
#include "api.h"
#include "adc.h"
#include "battery.h"
#include "carid.h"
#include "drive.h"
#include "fram.h"
#include "interrupts.h"
#include "linesensor.h"
#include "seg.h"
#include "us.h"
#include "us_i2c.h"
#include "util.h"
#include "serial.h"
#include "printf_stdarg.h"
#include "../cartography/create_map.h"
#include "../gps/navigation.h"
#include "wireless/wireless.h"
#include "../gps/BitwiseMap.h"
#include "../gps/mapMatrix.h"
#include "../gps/GPS_Handler.h"
#include "math.h"
#include "../gps/commonLibraries.h"

/* Demo task priorities. */
#define WTC_TASK_PRIORITY			( tskIDLE_PRIORITY + 5 )
#define mainXXX_PRIORITY	( tskIDLE_PRIORITY )
portTickType wakeLeft, wakeRight, wakeFront, wakeRear;
boolean_t getOnlyDrivenMap = FALSE;
xSemaphoreHandle Sema = NULL;

/*-----------------------------------------------------------*/

boolean_t drive_barrierFrontDetected(void) {
	return Us_Data.Front_DataValid && (Us_Data.Front_Distance < 30);
}

boolean_t drive_barrierRearDetected(void) {
	return Us_Data.Rear_DataValid && (Us_Data.Rear_Distance < 30);
}

void api_init(void) {
	CILCR_LVL = 0;
	interrupts_init();

	// button
	PIER07_IE0 = 1;
	PIER07_IE1 = 1;
	PIER07 |= 0x0c;
	PUCR07 |= 0x0c;

	Sema = 0;
	vSemaphoreCreateBinary(Sema);

	Seg_Init();
	Drive_Init();

	FRAM_Init();

	Us_Init();
	CarId_Init();
	ADC_Init();
	Battery_Init();
	Linesensor_Init();
#ifdef GPS_SCENARIO
	init_obstacles();
#endif

	wirelessInit();

	Serial_Init();

#ifdef NAVIGATION_THREAD
	navigationInit();
#endif
//?	BitwiseMap_init();

	__EI();
}

/**
 * SensorRange = einschränkung der Reichweite. Verhindert unnötig lange Berechnung
 * Return: Distance
 */
uint16_t driven_before(int8_t type, uint16_t SensorRange) {
	double diffR;
#ifdef CARTOGRAPHY_SCENARIO
	gps_reducedData_t *own;
	uint16_t angle, diffL;
	int16_t xCollision = 0, yCollision = 0, ownX, ownY;
	int32_t newX, newY;

	//Aktuelle Orientierung beachten!!! Orientierung in Abhängigkeit der Fahrzeugorientierung und dem aktuell betrachteten Sensor
	own = get_ownCoords();
	switch(type) {
		case RIGHT_SENSOR:
		angle = AngleMinus(own->angle, 16383);
		if (SensorRange > RANGESIDE*2) SensorRange = RANGESIDE*2; //siehe Anforderungen in wallFollow für align = 2
		break;
		case LEFT_SENSOR:
		angle = AnglePlus(own->angle, 16383);
		if (SensorRange > RANGESIDE*2) SensorRange = RANGESIDE*2;//siehe Anforderungen in wallFollow für align = 2
		break;
		case FRONT_SENSOR:
		angle = own->angle;
		if (SensorRange > RANGESIDE*2) SensorRange = RANGESIDE*2;
		break;
		default: angle = own->angle; break;
	}
	SensorRange = SensorRange / 5;

	ownX = CoordinatesToMap(own->x);
	ownY = CoordinatesToMap(own->y);

	if (Sema != NULL) {
		while ( xSemaphoreTake( Sema, ( portTickType ) 10 ) != pdTRUE ) {
			os_wait(10);
		}
		newX = (SensorRange*cos_Taylor(uint16DegreeRelativeToX(angle)))/100 + ownX;
		ownX = (2*cos_Taylor(uint16DegreeRelativeToX(angle)))/100 + ownX; //Ein stück aus dem Urpsrung rausgehen
		newY = (SensorRange*sin_Taylor(uint16DegreeRelativeToX(angle)))/100 + ownY;
		ownY = (2*sin_Taylor(uint16DegreeRelativeToX(angle)))/100 + ownY;

		//gibt es ein bereits befahrenes Gebiet/Hindernis das zwischen Sensorwert und Auto liegt?
		diffL = calc_reachability(&xCollision, &yCollision, ownX, ownY, (int16_t)newX, (int16_t)newY);

		xSemaphoreGive(Sema);
	} else return 600; //600 = max. Sensorreichweite

	if (diffL == 1) {
		diffR = sqrt(abs16(ownX - xCollision)*abs16(ownX - xCollision) + abs16(ownY - yCollision)*abs16(ownY - yCollision));
		diffR = diffR*5;
		if (diffR < 100) {
			Seg_Hex(0x00);
		}
		else Seg_Hex(0x01);

	}
	else {
		diffR = 600; // 600 für nichts gefunden
	}
	if (diffR > 600) diffR = 600;
#endif
	return (uint16_t) diffR;
}

/**
 * @unkownartist
 * Gibt die Distanz des vorderen Sensors zurück.
 */
uint16_t us_getFrontDistance(void){
	return Us_Data.Front_Distance;
}

/**
 * @unkownartist
 * Gibt die Distanz des linken Sensors zurück.
 */
uint16_t us_getLeftDistance(void){
	return Us_Data.Left_Distance;
}

/**
 * @unkownartist
 * Gibt die Distanz des rechten Sensors zurück.
 */
uint16_t us_getRightDistance(void){
	return Us_Data.Right_Distance;
}



/**
 * Wertet die Ultraschallsensoren und die befahrene Karte aus. Der minimale Abstand bis zu einer Kollision wird
 * in das struct UsSensorData geschrieben
 */
UsSensorData_t us_getSensorMapData(int8_t type) {
	UsSensorData_t result;
	uint16_t distance;

	result = Us_Data;

	getOnlyDrivenMap = TRUE;
	switch (type) {
		case RIGHT_SENSOR:
			if (wakeRight - os_getTime() > 100) {
				distance = driven_before(RIGHT_SENSOR, result.Right_Distance);
				if (distance < result.Right_Distance)
					result.Right_Distance = distance;
				wakeRight = os_getTime();
			}
			break;
		case LEFT_SENSOR:
			if (wakeLeft - os_getTime() > 100) {
				distance = driven_before(LEFT_SENSOR, result.Left_Distance);
				if (distance < result.Left_Distance)
					result.Left_Distance = distance;
				wakeLeft = os_getTime();
			}
			break;
		case FRONT_SENSOR:
			if (wakeFront - os_getTime() > 100) {
				distance = driven_before(FRONT_SENSOR, result.Front_Distance);
				if (distance < result.Front_Distance)
					result.Front_Distance = distance;
				wakeFront = os_getTime();
			}
			break;
		case REAR_SENSOR:
			if (wakeRear - os_getTime() > 100) {
				distance = driven_before(REAR_SENSOR, result.Rear_Distance);
				if (distance < result.Rear_Distance)
					result.Rear_Distance = distance;
				wakeRear = os_getTime();
			}
			break;
	}
	getOnlyDrivenMap = FALSE;

	return result;

}

void api_StartScheduler(void) {
	/* Now start the scheduler.  Following this call the created tasks should
	 be executing. */
	vTaskStartScheduler();

	/* vTaskStartScheduler() will only return if an error occurs while the
	 idle task is being created. */
	for (;;)
		;
}

xTaskHandle os_registerProcess(void(*function)(void), char * name) {
	return os_registerProcessStack(function, name, configMINIMAL_STACK_SIZE);
}

xTaskHandle os_registerProcessStack(void(*function)(void), char * name, uint16_t stackSize) {
	return os_registerProcessStackPriority(function, name, stackSize, tskIDLE_PRIORITY + 5);
}

xTaskHandle os_registerProcessStackPriority(void(*function)(void), char * name, uint16_t stackSize, uint16_t priority) {
	xTaskHandle xHandle;
	stackSize = stackSize / 2; //Byte to Word

	xTaskCreate((pdTASK_CODE)function, (const signed char*)name, stackSize, NULL, priority, &xHandle);
	return xHandle;
}

void os_deleteTask(xTaskHandle xHandle) {
	vTaskDelete(xHandle);
}

void os_suspendTask(xTaskHandle xHandle) {
	vTaskSuspend(xHandle);
}

void os_resumeTask(xTaskHandle xHandle) {
	vTaskResume(xHandle);
}

int16_t os_isTaskSuspended(xTaskHandle xHandle) {
	return xTaskIsTaskSuspended(xHandle);
}

void os_wait(int waitTimeInMs) {
	vTaskDelay(waitTimeInMs / portTICK_RATE_MS);
}

void os_frequency(portTickType * const PreviousWakeTime, int frequencyInMs) {
	vTaskDelayUntil(PreviousWakeTime, (portTickType) (frequencyInMs / portTICK_RATE_MS));
}

portTickType os_getTime(void) {
	return xTaskGetTickCount();
}
