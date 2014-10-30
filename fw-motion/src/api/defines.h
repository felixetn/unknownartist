/**
 * (c) Real-time systems project seminar, TU Darmstadt
 *
 * @file defines.h
 * @author Tobias
 * @brief Dieser Header beschreibt die Softwareproduktlinie und bietet die Möglichkeit
 * einzelne Szeanrien zu aktivieren
 **/

#ifndef DEFINES_H_
#define DEFINES_H_

/**
 * Definieren des Szenarios
 */
//#define NAVIGATION_SCENARIO
#define	ANDROID_SCENARIO
//#define GPS_SCENARIO
//#define	PREDATOR_SCENARIO
//#define MOTION_SCENARIO
//#define RF_SCENARIO
//#define HIGHWAY_SCENARIO
//  #define CARTOGRAPHY_SCENARIO
//#define SLAM_SCENARIO


#ifdef SLAM_SCENARIO
	#define GPS_SCENARIO
	#define LOCALIZATION
	#define CREATEMAP_TASK
//	#define BATTERY_TASK
	#define US_TASK
	#define SD_TASK
	#define CROSSINGANALYZER_TASK
	#define SEND_MAP
	#define IR_TASK
#endif

#ifdef CARTOGRAPHY_SCENARIO
	#define GPS_SCENARIO
	#define CREATEMAP_TASK
//	#define BATTERY_TASK
	#define US_TASK
	#define CROSSINGANALYZER_TASK
	#define SD_TASK
	#define SEND_MAP
	#define IR_TASK
#endif

#ifdef GPS_SCENARIO
	#define DRIVE_TASK
	#define NAVIGATION_THREAD
	#define AMB8420_TASK
	#define WIRELESS_TASK //Send with RF Module
#endif

#ifdef NAVIGATION_SCENARIO
	#define DRIVE_TASK
	#define CROSSINGANALYZER_TASK
	#define WALLFOLLOW_TASK
	#define MAPPING_TASK
	#define BATTERY_TASK
	#define MODECONTROLLER_TASK
	#define UPDATEDRIVETIME_TASK
	#define LINEFOLLOW_TASK
	#define BARCODE_TASK
	#define LINESENSOR_TASK
	#define CARID_TASK
	#define US_TASK
	#define SD_TASK
#endif

#ifdef HIGHWAY_SCENARIO
	#define DRIVE_TASK
	#define LINESENSOR_DEMO
	#define LINESENSOR_TASK
	#define IR_TASK
#endif

#ifdef PREDATOR_SCENARIO
	#define DRIVE_TASK
	#define IR_TASK
	#define LINESENSOR_TASK
	#define US_TASK
	#define BLUETOOTH_TASK
#endif

#ifdef MOTION_SCENARIO
	#define DRIVE_TASK
	#define IR_TASK
#endif

#ifdef ANDROID_SCENARIO
	#define DRIVE_TASK
	#define IR_TASK
	#define US_TASK
	#define BLUETOOTH_TASK
	#define AMB8420_TASK
	#define WIRELESS_TASK //Send with RF Module
	#define LINESENSOR_TASK
	#define BATTERY_TASK
#endif

#ifdef RF_SCENARIO
	#define AMB8420_TASK
	#define WIRELESS_TASK //Send with RF Module
#endif

//Allgemeine Definitionen
#define HALL_RESOLUTION 5.25
#define DRIVELENGTH 30

//DEBUG
//#define BARCODE_READER_DEBUG
//#define INITVALUES_DEBUG
//#define LINESENSOR_DEBUG
//#define MAP_DEBUG
//#define RF_DEBUG
//#define CROSSING_DEBUG
//#define MAPPINGCONTROLLER_DEBUG
//#define WALLFOLLOW_DEBUG
//#define SDCARD_DEBUG
//#define SIDEUS_DEBUG

#endif /* DEFINES_H_ */
