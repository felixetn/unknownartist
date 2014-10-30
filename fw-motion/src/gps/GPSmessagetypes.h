/**
 * (c) Real-time systems project seminar, TU Darmstadt
 *
 * @file GPSmessagetypes.h
 * @author TEAM::GPS
 * @brief contains
 * 			-all the structs for communication
 *			-the operation states of the system
 *			-constants
 *			-global variables
 **/


#ifndef GPSMESSAGETYPES_H
#define GPSMESSAGETYPES_H

#include "../api/base_type.h"

//the states of the TEAM::GPS car software:
#define GPS_STATE_IDLE 0
#define GPS_STATE_NAVIGATING 1


#define UNDEFINED_ID 255

//possible CONTROL-messages
#define CONTROL_YOUR_ID 0
#define CONTROL_START 1
#define CONTROL_STOP 2
#define CONTROL_FOLLOW_THIS_ID 3
#define CONTROL_FOLLOW_LOWEST_ID 4
#define CONTROL_FOLLOW_OFFROAD 5
#define CONTROL_FOLLOW_OFFROAD_DYN 6
#define CONTROL_CARTOGRAPHY 7
#define CONTROL_SLAM 8

typedef struct {
	uint16_t counter;
	uint8_t cam_id;
	uint8_t tag_id;
	int16_t x;
	int16_t y;
	uint16_t angle;
	uint8_t isWorld; //bool: isWorld=->world coordinates; isWorld=0 -> relative coordinates
	float x_velocity;
	float y_velocity;
	float rotation_velocity;
	float motion_accelleration;
	float rotation_accelleration;
} gps_dataPacket_t;

typedef struct {
		uint16_t counter;
		uint8_t cam_id;
		uint8_t tag_id;
		int16_t x;
		int16_t y;
		uint16_t angle;
		uint8_t isWorld;
} gps_reducedData_t;

typedef struct{
		uint8_t ctrlMsgType;
		uint8_t data;
} gps_controlMessage_t;


#endif
