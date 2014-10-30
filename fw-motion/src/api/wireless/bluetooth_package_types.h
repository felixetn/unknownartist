/**
 * (c) Real-time systems project seminar, TU Darmstadt
 *
 * @file bluetooth_package_types.h
 * @author Administrator
 * @brief TODO - insert a brief description of that interface
 **/

#ifndef BLUETOOTH_PACKAGE_TYPES_H_
#define BLUETOOTH_PACKAGE_TYPES_H_

#define PT_ANDROID_CONTROL	0
#define PT_ANDROID_SENSORDATA	1
#define PT_CALIBRATE_SENSORS	2
#define PT_CALIBRATE_LINESENSORS	3

#define PT_GPS_CONTROL 10
#define PT_GPS_UPDATE_COORD 11
#define PT_GPS_DEBUG_INFO 12

#define PT_PREDATOR_HEARTBEAT	20
#define PT_PREDATOR_FULLSTATE	21
#define PT_PREDATOR_AXISSTATE	22
#define PT_PREDATOR_BUTTONSTATE	23
#define PT_PREDATOR_HAPTIC_RUMBLE	24
#define PT_PREDATOR_HAPTIC_FORCE	25
#define PT_PREDATOR_STATUS	26
#define PT_DRIVE_CONTROL	27


#endif /* BLUETOOTH_PACKAGE_TYPES_H_ */
