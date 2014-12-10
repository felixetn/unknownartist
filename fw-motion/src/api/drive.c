#include "drive.h"
#include "base_type.h"
#include "FreeRTOS.h"
#include "api.h"
#include "interrupts.h"
#include "carid.c" // Für die ID!!! Nicht löschen

#include "wireless/wireless.h"

/*****************************************************************************/
/* Local pre-processor symbols/macros ('#define')                            */
/*****************************************************************************/
#define PPG_MOTOR_NULL 1488
#define PPG_MOTOR_RANGE 200
#define PPG_SERVO_NULL 1470
#define PPG_SERVO_RANGE 300

/*****************************************************************************/
/* Global variable definitions (declared in header file with 'extern')       */
/*****************************************************************************/
volatile int8_t Drive_Speed = 0;
volatile int8_t Drive_Servo = 0;
static volatile uint32_t Drive_TotalDistance = 0;

/*****************************************************************************/
/* Local variable definitions ('static')                                     */
/*****************************************************************************/
static uint16_t ppg_motor_values[] = { 0, 79, 84, 93, 105, 120, 140, 170, 200, 250, 270 };
static uint8_t ID ;

//static volatile uint16_t ppg_motor_dest = PPG_MOTOR_NULL;
//static volatile uint16_t ppg_motor_curr = PPG_MOTOR_NULL;
//static volatile uint16_t ppg_servo_dest = PPG_SERVO_NULL;
//static volatile uint16_t ppg_servo_curr = PPG_SERVO_NULL;

//static volatile boolean_t ppg_motor_last_positive = 0;

static volatile uint16_t lastReloadValue = 60000;

/*****************************************************************************/
/* Local function prototypes ('static')                                      */
/*****************************************************************************/
static void Drive_Systick(void);

static __interrupt
void Drive_DistanceInterrupt(void);

/*****************************************************************************/
/* Function implementation - global ('extern') and local ('static')          */
/*****************************************************************************/

#ifndef DISABLE_SECURITY_STOP
static void drive_securityStop(void) {
	portTickType lastWakeTime = os_getTime();

	for (;;) {
		os_frequency(&lastWakeTime, 10);

		if ((drive_barrierFrontDetected() && (Drive_Speed > 0)) || (drive_barrierRearDetected() && (Drive_Speed < 0))) {
			Drive_SetMotor(0);
		}
	}
}
#endif

static int16_t getInRange(int16_t val, int16_t min, int16_t max) {
	if (val < min)
		return min;
	if (val > max)
		return max;
	return val;
}

static void setCounter(uint16_t cnt) {
	uint16_t val = TMR1;
	if (val != 0xffff)
		Drive_TotalDistance += lastReloadValue - TMR1;
	lastReloadValue = cnt - 1;
	TMRLR1 = cnt - 1;
	TMCSR1_TRG = 1;
}

void Drive_SetServo(int8_t percent) {

	//Thanks to Artur And The Chipmunks
	//Offset um einheitlich für die Autos zu setzen , damit sie alle grade fahren
	switch(carid){
		case 1:
			steeringOffset = 17.77 * (PPG_SERVO_RANGE / 100); // 17,77
			break;
		case 2:
			steeringOffset = 25.26 * (PPG_SERVO_RANGE / 100); // 25,26
			break;
		case 3:
			steeringOffset = -6.08 * (PPG_SERVO_RANGE / 100); //-6,08
			break;
		case 4:
			steeringOffset = 16.94 * (PPG_SERVO_RANGE / 100); //16,94
			break;
		case 5:
			steeringOffset = -1.23 * (PPG_SERVO_RANGE / 100); //-1,23
			break;
		case 6:
			steeringOffset = 7.99 * (PPG_SERVO_RANGE / 100); //7,99
			break;
		case 7:
			steeringOffset = 0;
			break;
	}

	Drive_Servo = getInRange(percent+steeringOffset, -100, 100);

}

int8_t Drive_GetServo()
{
	return Drive_Servo;
}

void Drive_SetMotor(int8_t power) {
	setCounter(60000);
	Drive_Speed = getInRange(power, -10, 10);
}

int8_t Drive_GetMotor()
{
	return Drive_Speed;
}

void Drive_SetMotorForDistance(int8_t power, uint32_t distance) {
	setCounter(distance / 25);
	Drive_Speed = getInRange(power, -10, 10);
}

void Drive_Init(void) {
	DDR06_D0 = 1;
	DDR06_D1 = 1;

	// PPG0 -> Servo Output
	PCN0 = 0;
	PCN0_MDSE = 0;
	PCN0_RTRG = 0; //?
	PCN0_CKS0 = 0;
	PCN0_CKS1 = 1; // Clk/16
	PCN0_PGMS = 0;
	PCN0_EGS = 0; //?
	PCN0_IREN = 0;
	PCN0_IRS = 0;
	PCN0_OE = 1;
	PCN0_OSEL = 0;

	// PPG1 -> Motor Output
	PCN1 = 0;
	PCN1_MDSE = 0;
	PCN1_RTRG = 0; //?
	PCN1_CKS0 = 0;
	PCN1_CKS1 = 1; // Clk/16
	PCN1_PGMS = 0;
	PCN1_EGS = 0; //?
	PCN1_IREN = 0;
	PCN1_IRS = 0;
	PCN1_OE = 1;
	PCN1_OSEL = 0;

	GCN10 = 0;
	GCN20 = 0;

#define PPG_DRIVE_CYCLE 17429
	PCSR0 = PPG_DRIVE_CYCLE;
	PCSR1 = PPG_DRIVE_CYCLE;

	PDUT0 = PPG_SERVO_NULL;
	PDUT1 = PPG_MOTOR_NULL;

	PCN0_CNTE = 1;
	PCN0_STGR = 1;
	PCN1_CNTE = 1;
	PCN1_STGR = 1;

	PIER01_IE0 = 1;
	DDR01_D0 = 0;
	TMCSR1_FSEL = 1;
	TMCSR1_CSL = 3; // Event count mode
	TMCSR1_MOD2 = 0; // Both edges
	TMCSR1_MOD1 = 1;
	TMCSR1_MOD0 = 1;
	TMCSR1_OUTE = 0;
	TMCSR1_RELD = 0; // Reload
	TMCSR1_UF = 0; // Clear underflow
	TMCSR1_INTE = 1; // Interrupt
	TMCSR1_CNTE = 1; // Enable counting
	TMRLR1 = lastReloadValue; // Initialise reload value
	TMCSR1_TRG = 1; // Load counter

#ifdef DRIVE_TASK
	os_registerProcess(Drive_Systick, "Drive systick wrapper");
	interrupts_setHandler(52, 6, Drive_DistanceInterrupt);
#endif
#ifndef DISABLE_SECURITY_STOP
	os_registerProcess(drive_securityStop, "SecurityStop");
#endif
}

uint32_t Drive_GetTotalDrivenDistance(void) {
	uint16_t val = TMR1;
	if (val == 0xffff)
		return Drive_TotalDistance * 25;
	else
		return (Drive_TotalDistance + (lastReloadValue - TMR1)) * 25;
}

static __interrupt
void Drive_DistanceInterrupt(void) {
	TMCSR1_UF = 0;
	Drive_TotalDistance += lastReloadValue + 1;
	Drive_Speed = 0;
	PDUT1 = PPG_MOTOR_NULL;
	setCounter(60000);
}

static void setMotorDirect(uint16_t duty) {
	PDUT1 = getInRange(duty, PPG_MOTOR_NULL - PPG_MOTOR_RANGE, PPG_MOTOR_NULL + PPG_MOTOR_RANGE);
}

static void Drive_Systick(void) {
	static boolean_t ppg_motor_last_positive = TRUE;
	static int8_t lastPower = 0;
	static int8_t lastServo = 0;
	portTickType lastWakeTime;

	lastWakeTime = os_getTime();

	for (;;) {
		os_frequency(&lastWakeTime, 5);

		if (Drive_Speed != lastPower) {
			lastPower = Drive_Speed;
			if (lastPower < 0) {
				if (ppg_motor_last_positive == TRUE)
				{
					setMotorDirect(PPG_MOTOR_NULL + ppg_motor_values[3]);
					os_wait(100);
					setMotorDirect(PPG_MOTOR_NULL);
					os_wait(100);
				}
				setMotorDirect(PPG_MOTOR_NULL + ppg_motor_values[-lastPower]);
				ppg_motor_last_positive = FALSE;
			} else {
				setMotorDirect(PPG_MOTOR_NULL - ppg_motor_values[lastPower]);
				if (lastPower > 0)
					ppg_motor_last_positive = TRUE;
			}
		}
		if (Drive_Servo != lastServo) {
			PDUT0 = getInRange(Drive_Servo * (PPG_SERVO_RANGE / 100) + PPG_SERVO_NULL, PPG_SERVO_NULL - PPG_SERVO_RANGE
					, PPG_SERVO_NULL + PPG_SERVO_RANGE);
			lastServo = Drive_Servo;
		}
	}
}
