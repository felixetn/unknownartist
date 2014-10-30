/**
 * (c) Real-time systems project seminar, TU Darmstadt
 *
 * @file wall_follow.c
 * @author pnotz
 * @brief MappingController creates maps by getting information about the nodes from CrossingAnalyzer
 *        and collecting information about the edges. Mapping Controller is also responsible
 *        for defining the behavior for every CrossingType. This could be expanded by implementing
 *        AI so that the car is able to recognize crossings it has already visited and take
 *        another route.
 **/


#include "mapping_controller.h"
#include "crossing_analyzer.h"
#include "../api/api.h"
#include "../api/us.h"
#include "initValues.h"
#include "dataOutput.h"
#include "calibDrive.h"
#include "wall_follow.h"
#include "../gps/map.h"

nodeNumber currentNodeNumber = 0;
int8_t last_edge_side = 0;

xTaskHandle xHandleMappingController;

uint8_t us_values[2 * NUMBER_US_VALUES];

void initMappingController(void) {
	uint8_t i;
	xHandleMappingController = 0;

	// init us_values array with zeros
	for (i = 0; i < 2 * NUMBER_US_VALUES; i++)
		us_values[i] = 0;
}

void startMappingController(void) {
#ifdef MAPPING_TASK
	if (xHandleMappingController != 0)
		os_resumeTask(xHandleMappingController);
	else
		xHandleMappingController = os_registerProcessStack(MappingControllerThread,"MappingController",900);
#endif
}

void stopMappingController(void) {
	if (xHandleMappingController)
		os_suspendTask(xHandleMappingController);
}

void MappingControllerThread(void) {

	int16_t backLength; // the distance to the center of a crossing
	int16_t w_right, w_left, w_front, w_rear; // the widths of a crossing

	// needed for collecting us sensor data for the information about edges
	int8_t us_pos = 0;
	uint8_t wait_period = 1;
	int8_t k = 1;

	map_node_info node_info; // info about the crossing that shall be saved in the map

	for (;;) {
	//	Seg_Hex(0x8);
		os_wait(250);

		// description of the algorithm for collecting this data:
		// for every edge each 10 ( = NUMBER_US_VALUES) values of us left and right sensor data
		// shall be saved. Therefore the array us_values is filled alternately with
		// left (even positions) and right (odd positions) us sensor data. If the array is full,
		// every second value will be deleted and afterwards only every second measured
		// us sensor data is saved in the array to keep the distance between two values constant.
		// and so on.
		// --> The array will always be at least half-full and contain us sensor values with
		// equal distances between them.
		if (k == wait_period && getWidthFront()==0) {
			// collect us values for edge information
			if (us_pos < 2 * NUMBER_US_VALUES) {
				us_values[us_pos] = Us_Data.Left_Distance; // even value: left
				us_values[us_pos + 1] = Us_Data.Right_Distance; // odd value: right
				us_pos += 2;
			} else {
				// take every second value and increment the wait period
				// the array is half-full afterwards
				uint8_t n;
				for (n = 0; n < NUMBER_US_VALUES; n += 2) {
					us_values[n] = us_values[2 * n];
					us_values[n + 1] = us_values[2 * n + 1];
				}
				us_pos /= 2;
				wait_period = wait_period<<=1; // wait_period * 2
			}
			k = 1; // reset k
		} else
			k++;

		// ---------------------
		// START of crossing
		if (getCrossingStarted() == 1) {

			uint8_t edge_length;

			// calculate length of last edge
			edge_length = calculateDrivenLen(os_getTime() - getTimeEdgeOld());

			// add edge to map and increase the currentNodeNumber
			addEdge(currentNodeNumber, ++currentNodeNumber, edge_length);

			// reset value
			resetCrossingStarted();
		}

		// ---------------------
		// END of crossing
		if (getCrossingDetected() == 0)
			continue;
#ifdef MAPPINGCONTROLLER_DEBUG
		printf("Crossing detected, MappingController is handling it...\r\n");
#endif

		stopCrossingAnalyzer();
		stopWallFollow();

		// get the widths and add this information to the node info
		w_left = getWidthLeft();
		w_right = getWidthRight();
		w_front = getWidthFront();
		w_rear = getWidthRear();

		node_info.w0 = w_front;
		node_info.w1 = w_left;
		node_info.w2 = w_rear;
		node_info.w3 = w_right;

		// Filter open doors etc.
		// the width of a corridor must be bigger than 40cm
		if (w_front < 40)
			w_front = 0;
		if (w_rear < 40)
			w_rear = 0;
		if (w_left < 40)
			w_left = 0;
		if (w_right < 40)
			w_right = 0;

		// Drive backwards to center of crossing
		if (w_left != 0 && w_right != 0)
			backLength = (w_right + w_left) / 4;
		else
			backLength = (w_right + w_left) / 2;
		if (backLength == 0) // dead end
			backLength = 20;
		if (w_rear == 0)
			backLength -= 20;
		backLength *= -1; // backwards driving
#ifdef MAPPINGCONTROLLER_DEBUG
		printf("Driving %dcm\r\n", backLength);
#endif

		driveLen(backLength);

		// Handle different crossing types
		// width_front = direction the car came from
		// width_rear = direction the car was heading to
		// side_end: 1: coming up, 2: left, 3: straight, 4: right
		// add information about the edge to the map and resume driving
		if (w_left == 0 && w_right == 0 && w_front != 0 && w_rear == 0) {
			noteCrossing(DeadEnd);
			addEdgeInfo(last_edge_side, 1, NUMBER_US_VALUES, us_values);
			last_edge_side = 1;
			node_info.crossing_type = DeadEnd;
			turn180(RIGHT, BACKWARDS);
			resumeDriving(0);
		} else if (w_left != 0 && w_right == 0 && w_front != 0 && w_rear != 0) {
			noteCrossing(LeftStraight);
			addEdgeInfo(last_edge_side, 3, NUMBER_US_VALUES, us_values);
			last_edge_side = 3;
			node_info.crossing_type = LeftStraight;
			// Behavior: drive straight ahead!
			resumeDriving(w_left / 2 + 50);
		} else if (w_left == 0 && w_right != 0 && w_front != 0 && w_rear != 0) {
			noteCrossing(RightStraight);
			addEdgeInfo(last_edge_side, 4, NUMBER_US_VALUES, us_values);
			last_edge_side = 4;
			node_info.crossing_type = RightStraight;
			turn90(RIGHT, BACKWARDS);
			resumeDriving(w_rear / 2 + 50);
		} else if (w_left == 0 && w_right != 0 && w_front != 0 && w_rear == 0) {
			noteCrossing(RightOnly);
			addEdgeInfo(last_edge_side, 4, NUMBER_US_VALUES, us_values);
			last_edge_side = 4;
			node_info.crossing_type = RightOnly;
			turn90(RIGHT, BACKWARDS);
			resumeDriving(w_rear / 2 + 50);
		} else if (w_left != 0 && w_right == 0 && w_front != 0 && w_rear == 0) {
			noteCrossing(LeftOnly);
			addEdgeInfo(last_edge_side, 2, NUMBER_US_VALUES, us_values);
			last_edge_side = 2;
			node_info.crossing_type = LeftOnly;
			turn90(LEFT, BACKWARDS);
			resumeDriving(w_rear / 2 + 50);
		} else if (w_left != 0 && w_right != 0 && w_front != 0 && w_rear == 0) {
			noteCrossing(LeftRight);
			addEdgeInfo(last_edge_side, 2, NUMBER_US_VALUES, us_values);
			last_edge_side = 2;
			node_info.crossing_type = LeftRight;
			turn90(LEFT, BACKWARDS);
			resumeDriving(w_rear / 2 + 50);
		} else if (w_left != 0 && w_right != 0 && w_front != 0 && w_rear != 0) {
			noteCrossing(All);
			addEdgeInfo(last_edge_side, 3, NUMBER_US_VALUES, us_values);
			last_edge_side = 3;
			node_info.crossing_type = All;
			// Behavior: drive straight ahead!
			resumeDriving(w_left / 2 + 50);
		} else { // dunno
			resumeDriving(0);
		}

		// add information about the current node
		addNodeInfo(currentNodeNumber, node_info);
		// reset values
		for (k = 0; k < 2 * NUMBER_US_VALUES; k++) {
			us_values[k] = 0;
		}
	}
}

void noteCrossing(CrossingType type) {
	printf("Denoting Crossing of type %d\r\n", type);
}

void resumeDriving(int16_t len) {
#ifdef MAPPINGCONTROLLER_DEBUG
	printf("Waiting 500ms before continuing\r\n");
	os_wait(500);
	printf("Resuming driving %d\r\n", len);
#endif

	// drive straight ahead to avoid edges
	drive_setSteeringAngleBalanced(0);
	driveLen(len);

	// restart CrossingAnalyzer and WallFollow
	startCrossingAnalyzer();
	startWallFollow(2, 0);
}

