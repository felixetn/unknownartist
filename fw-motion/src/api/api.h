/**
 * \file api.h
 * \brief API to use the car components.
 *
 * \date 2010-05-28
 * \author Team Motion
 */

#ifndef API_H_
#define API_H_

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "defines.h"
#include "base_type.h"

#include "portmacro.h"

/**
 * If flag DISABLE_SECURITY_STOP is set, car will not prevent from colliding with objects
 */
#define DISABLE_SECURITY_STOP

extern xSemaphoreHandle Sema;

/**
 * Enumeration der Sensoren
 */
typedef enum SensorDirection{
	LEFT_SENSOR = 0,
	RIGHT_SENSOR = 1,
	FRONT_SENSOR = 2,
	REAR_SENSOR = 3
}SensorDirection;

typedef enum StorageType{
	DRIVEN_AREA = 0,
	OBSTACLE = 1
}StorageType;

extern boolean_t getOnlyDrivenMap;

/**
 * Struct for Ultrasonic Sensor's Data
 * DataValid: determines, whether the values are up-to-date and valid
 * Distance: distance to the detected object in centimeters
 */
typedef struct {
	boolean_t Front_DataValid :1;
	boolean_t Rear_DataValid :1;
	boolean_t Left_DataValid :1;
	boolean_t Right_DataValid :1;
	uint16_t Front_Distance;
	uint16_t Rear_Distance;
	uint16_t Left_Distance;
	uint16_t Right_Distance;
} UsSensorData_t;

/**
 * SensorRange = einschränkung der Reichweite. Verhindert unnötig lange Berechnung
 * Return: Distance
 */
uint16_t driven_before(int8_t type, uint16_t SensorRange);

/**
 * @unkownartist
 * Gibt die Distanz des vorderen Sensors zurück.
 */
uint16_t us_getFrontDistance(void);

/**
 * @unkownartist
 * Gibt die Distanz des linken Sensors zurück.
 */
uint16_t us_getLeftDistance(void);

/**
 * @unkownartist
 * Gibt die Distanz des rechten Sensors zurück.
 */
uint16_t us_getRightDistance(void);

/**
 * Wertet die Ultraschallsensoren und die befahrene Karte aus. Der minimale Abstand bis zu einer Kollision wird
 * in das struct UsSensorData geschrieben
 */
UsSensorData_t us_getSensorMapData(int8_t type);

/**
 * Checks if there is an object in front of the car on collision course.<br>
 * Uses front ultrasonic sensor for distances up to 100cm and front infrared sensor for emergency detection.
 */
boolean_t drive_barrierFrontDetected(void);

/**
 * Checks if there is an object in rear of the car on collision course.<br>
 * Uses rear ultrasonic sensor for distances up to 100cm and rear infrared sensor for emergency detection.
 */
boolean_t drive_barrierRearDetected(void);

/**
 * This function should be called by the operating system to initialize all external
 * interfaces, sensors and actors.
 */
void api_init(void);

/**
 * Causes the operating system to pause the given task for the timespan * milliseconds.
 * @param waitTimeInMs is the time to wait until the OS should add the thread to the
 * readyToRun-Queue.
 */
void os_wait(int waitTimeInMs);

/**
 * Causes the operating system to pause the given task for the timespan * milliseconds
 * till the last time running.
 * @param PreviousWakeTime last wake up time
 * @param frequencyInMs periodical time
 */
void os_frequency(portTickType * const PreviousWakeTime, int frequencyInMs);

/**
 * Help function to get the current time of the system.
 * @return returns the current system time
 */
portTickType os_getTime(void);

/**
 * Starts the Scheduler. It will only return if an error occurs while the
 *	idle task is being created
 */
void api_StartScheduler(void);

/**
 * Adds a task to the scheduler of the operation system with default stack size
 * (180 Byte). This task is called periodically. Have a look at the examples
 * section.
 * @param function Function-pointer of task which will be called
 * @param name Name of the Process
 * @return Handle of task (for suspending / resuming)
 */
xTaskHandle os_registerProcess(void(*function)(void), char * name);

/**
 * Adds a task to the scheduler of the operation system with given stack size.
 * This task is called periodically.
 * @param function Function-pointer of task which will be called
 * @param name Name of the Process
 * @param stackSize Size of Stack in Bytes
 * @return Handle of task (for suspending / resuming)
 */
xTaskHandle os_registerProcessStack(void(*function)(void), char * name,
		uint16_t stackSize);

/**
 * Adds a task to the scheduler of the operation system with given stack size and priority.
 * This task is called periodically.
 * @param function Function-pointer of task which will be called
 * @param name Name of the Process
 * @param stackSize Size of Stack in Bytes
 * @param priority Priority of the task
 * @return Handle of task (for suspending / resuming)
 */
xTaskHandle os_registerProcessStackPriority(void(*function)(void), char * name,
		uint16_t stackSize, uint16_t priority);

/**
 * Remove a task from the RTOS real time kernels management.  The task being
 * deleted will be removed from all ready, blocked, suspended and event lists.
 *
 * @param pxTask The handle of the task to be deleted.  Passing NULL will
 * cause the calling task to be deleted.
 */
void os_deleteTask(xTaskHandle xHandle);

/**
 * Suspend the task given by the handle. Passing NULL will suspend the calling
 * task.
 * @param xHandle task that will be suspended
 */
void os_suspendTask(xTaskHandle xHandle);

/**
 * Resume the task given by the handle.
 * @param xHandle task that will be resumed
 */
void os_resumeTask(xTaskHandle xHandle);

/**
 * Is the task given by the handle suspended? A task is not considered suspended
 * if it has been blocked by some other task.
 * @param xHandle handle of the task
 * @return 0 if the task is not suspended, 1 if it is suspended
 */
int16_t os_isTaskSuspended(xTaskHandle xHandle);

#endif /* API_H_ */
