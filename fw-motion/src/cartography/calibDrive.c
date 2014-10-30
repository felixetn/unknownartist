/**
 * (c) Real-time systems project seminar, TU Darmstadt
 *
 * @file calibDrive.c
 * @author pnotz
 * @brief This file provides methods to move the car defined lengths or angles. This is possible
 * 		  by using measured (init) values and the battery level (move front, backward, turn)
 **/

#include "calibDrive.h"
#include "../gps/navigation.h"
#include "../api/linesensor.h"
#include "../api/battery.h"
#include "../api/drive.h"

#include "../gps/help_methods.h"
#include "../linesensorDemo.h"

//static boolean_t distanceReached = FALSE;
xTaskHandle xHandleDrive;
//static uint16_t DriveLength = 0;
/*
 * internal function
 * @return number of ticks to drive 5 meters with current battery level
 */
int getTicksPer5meters(void)
{
	return driveSpeedReg_n_Fac10*10 - driveSpeedReg_m * Battery_GetVoltage();
}

//void DrivenLength(void){
//	uint16_t wheel_turns;
//	int8_t start = PDR01_P0;
//	portTickType lastWakeTime;
//
//	lastWakeTime = os_getTime();
//
//
//	for (;;){
//		os_frequency(&lastWakeTime, 50); //55Hz -> 50 gerundet
//
//		if (DriveLength != 0){
//			wheel_turns = (DriveLength)/10.5;
//			//Zwei Sensorwerte = 1 Radumdrehung = 21,04cm
//			if ((DriveLength*10) % 105 > 52) wheel_turns++; //Faktor 10 weil kein Mod mit double möglich
//		//	storeElement(wheel_turns);
//			set_distanceReached(FALSE);
//		//	storeElement(DriveLength);
////			storeElement(wheel_turns);
//			if (Drive_Speed > 3 && wheel_turns > 2) wheel_turns = wheel_turns - 2;
//
//			while (wheel_turns > 0){
////				if (us_getSensorData().Front_Distance < 40) wheel_turns = 0;
//				if (PDR01_P0 != start){
//					start = PDR01_P0;
//					Seg_Hex(0x01);
//					wheel_turns--;
//	//				os_wait(300);
//				}
//				else{
//					Seg_Hex(0x07);
//					start = !start;
//				}
//				os_wait(10);
//			}
//			DriveLength = 0;
//			set_distanceReached(TRUE);
//		}
//
//	}
////	os_wait(300);
//
////	if (wheel_turns <= 0) return TRUE;
////	else return FALSE;
//}
//
//void start_DriveLength(){
//	set_distanceReached(FALSE);
//	if (xHandleDrive != NULL)
//		os_resumeTask(xHandleDrive);
//	else xHandleDrive = os_registerProcessStackPriority(DrivenLength, "Drive Length", 200, tskIDLE_PRIORITY + 5);
//}
//
//void stop_DriveLength(){
//	set_distanceReached(FALSE);
//	if (xHandleDrive)
//		os_suspendTask(xHandleDrive);
//}

/*
 * calculates the length the car drove within the time and with MotorSpeed = 1
 * @param time_in_ms the driven time in ms
 * @return the driven length in cm
 */
int calculateDrivenLen(portTickType time_in_ms) {
	return time_in_ms * ( 500.0f / getTicksPer5meters()) - 4;
}

portTickType getTimeforLength(int16_t len_in_cm){
	int ticksperway = getTicksPer5meters();
	return (ticksperway / 500) * (abs16(len_in_cm) + 4); //+4 to start going
}

/*
 * driving a defined length forwards (len>0) or backwards (len<0) - len in cm
 * (should only be called within a thread, because of os_wait usage)
 * @param len_in_cm the desired length to drive
 */
void driveLen(int16_t len_in_cm) {
	portTickType waitticks;
	int ticksperway = getTicksPer5meters();
	int direction = 2;

	//stop car first
//	Drive_SetMotor(0);
//	Drive_SetServo(0);
	drive(0,Drive_Servo,700);

	// calculate the (forward) driving time depending on battery level
	waitticks = (ticksperway / 500) * (abs16(len_in_cm) + 4); //+4 to start going

	if (len_in_cm < 0) // if going backwards
	{
		waitticks *= time_backward * 1.0 / time_forward;
		waitticks -= 8 * (ticksperway / 500); // instead of +4 it is -4 to start going
		direction = -2;
	}

	//drive
	Drive_SetMotor(direction);
	drive(Drive_Speed,Drive_Servo,700);
	vTaskDelay(waitticks);
//			storeElement(22);
//			storeElement(ticksperway);
//			storeElement(waitticks);
//			storeElement(-1);
//	vTaskDelay(3000);

	//stop car
	Drive_SetMotor(0);
	drive(Drive_Speed,Drive_Servo,700);
}

/*
 * wrapper method for drive_setSteeringAngle that takes the balanced steering angle into account
 * @param percent steeringAngle (>0 turn right, <0 turn left)
 */
void drive_setSteeringAngleBalanced(int8_t percent) {
	if (percent + balancedSteerAngle > 100)
		Drive_SetServo(100);
	else if (percent + balancedSteerAngle < -100)
		Drive_SetServo(-100);
	else
		Drive_SetServo(percent + balancedSteerAngle);
}

/*
 * measures the steering angle for driving straight
 * will drive on a straight line using line follow
 * the average steering angle will be used as neutral position
 */
void measure_SteeringAngle(void) {
	int currentAngle = 0;
	int repeats = 300;
	int i = 1;
	Drive_SetMotor(0);
	Drive_SetServo(0); // start position
//	drive(Drive_Speed,Drive_Servo,500);
	os_wait(100); //wait for servo

	Linesensor_Calibrate();

	Drive_SetMotor(2);
//	drive(Drive_Speed,Drive_Servo,500);

	for (;;) {
		if (linefound == 1) {
			steerToLine();
			currentAngle += Drive_Servo;
			i++;
		}
		if (i > repeats) { // enough values collected
			balancedSteerAngle = currentAngle / repeats;
			showOnSegmentDisplay(balancedSteerAngle, 700);
			break;
		}
		os_wait(5);
	}

	//stop car
	Drive_SetMotor(0);
//	drive(Drive_Speed,Drive_Servo,500);
}

/*
 * internal turn function
 * @param repeats number of turn cycles (forward+backward driving)
 * @param fac_turnDirection 1: turn right, -1: turn left
 * @param fac_firstDirection 1: forwards, -1: backwards
 */
void turnRepeats(int repeats, int8_t fac_turnDirection, int8_t fac_firstDirection)
{
	turn12(repeats, fac_turnDirection, fac_firstDirection);
/*	uint8_t i;
	uint16_t time_f = 200;//time_forward,
	uint16_t time_b = 160; //time_backward;

	if (fac_firstDirection != 1)
	{
		time_f = 160;//time_backward; // swap values
		time_b = 200;//time_forward;
	}

	//stop car
	Drive_SetMotor(0);
//	drive(Drive_Speed,Drive_Servo,500);

	// turns by driving forward/backward and turning the wheels
	 for (i=0; i < repeats; i++) {
	             drive_setSteeringAngleBalanced(100*fac_turnDirection);
	             os_wait(time_wait);
//	         	 drive(2*fac_firstDirection,Drive_Servo, 500);
	             Drive_SetMotor(2*fac_firstDirection); //
	             os_wait(time_f);
	             Drive_SetMotor(0);
	             os_wait(time_wait);
	             drive_setSteeringAngleBalanced(-100*fac_turnDirection);
	             os_wait(time_wait);
//	             drive(-2*fac_firstDirection, -Drive_Servo, 500);
	             Drive_SetMotor(-2*fac_firstDirection);
	             os_wait(time_b);
	             Drive_SetMotor(0);
	             os_wait(time_wait);
	       } */
/*
	for (i = 0; i < repeats; i++) {
		//steering balanced change the value between -25 and 25
		Drive_SetServo(75 * fac_turnDirection);
		os_wait(time_wait); // wait till the servo (steering wheel) is in the right position
		Drive_SetMotor(2 * fac_firstDirection); // move forward (2 because 1 is too slow)
//		drive(Drive_Speed,Drive_Servo,500);
		os_wait(time_f); //drive time
		Drive_SetMotor(0);
		os_wait(time_wait); //wait for car to stop
		Drive_SetServo(-75 * fac_turnDirection); // turn the servo in other direction
		os_wait(time_wait);
		Drive_SetMotor(-2 * fac_firstDirection); // move backward
//		drive(Drive_Speed,Drive_Servo,500);
		os_wait(time_b);
		Drive_SetMotor(0);
		os_wait(time_wait); //wait for car to stop
	}

	//reset steering direction
	drive_setSteeringAngleBalanced(0);
	os_wait(time_wait); */
}

/**
 * makes the car turn by a given angle (>0 to the right; <0 to the left)
 * the angle must be a multiple of five (rounded otherwise)
 * @param angle angle to be turned by
 */
void turn(int32_t angle) {
	//call internal turn function
	turnRepeats(abs8(angle) / 5, ((angle > 0)?1:-1), 1);
}

/**
 * makes the car turn 45 degrees
 * @param align whether it shall turn 45 degrees to the right (1) or to the left (0)
 * @param direction whether it shall drive forward (1) or backward (0) first
 */
void turn45(boolean_t align, boolean_t firstDirection) {
	//call internal turn function
	turnRepeats(2, ((align == RIGHT)?1:-1), ((firstDirection == FORWARDS)?1:-1));
}

/**
 * makes the car turn 90 degrees
 * @param align whether it shall turn 90 degrees right (1) or left (0)
 * @param direction whether it shall drive forward (1) or backward (0) first
 */
void turn90(boolean_t align, boolean_t firstDirection) {
	turnRepeats(3,align,firstDirection);
//	turn45(align, firstDirection);
//	turn45(align, firstDirection);
//	driveLen(-10);
//	drive(-2,0,700);
//	vTaskDelay(200);
//	drive(0,0,700);
}

/**
 * makes the car turn around (180 degrees)
 * @param align whether it shall turn 90 degrees right (1) or left (0)
 * @param direction whether it shall drive forward (1) or backward (0) first
 */
void turn180(boolean_t align, boolean_t firstDirection) {
	turn90(align, firstDirection);
	turn90(align, firstDirection);
}
