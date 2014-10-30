/**
 * (c) Real-time systems project seminar, TU Darmstadt
 *
 * @file barcode_reader.h
 * @author pnotz
 * @brief Barcode reader is able to detect line barcodes on white paper on a floor that is darker
 * 	      than the paper. It will switch from WallFollow to LineFollow as soon as a barcode is expected
 * 	      (white paper), it will read the barcode and then interpret it as defined in the set
 * 	      barcode handling funktion (setBarcodeHandlingFunction(.))
 **/

#ifndef BARCODE_READER_H_
#define BARCODE_READER_H_

#include "../api/api.h"
#include "../api/linesensor.h"
#include "../api/seg.h"
#include "calibDrive.h"
#include "line_follow.h"
#include "wall_follow.h"
#include "../api/printf_stdarg.h"
#include "initValues.h"
#include "dataOutput.h"
#include "../api/defines.h"


//using line sensor data to detect white paper
extern int16_t cal_sensor[SENSOR_COUNT];

//using line sensor variable if we have a line
extern uint8_t bcLineFound;

// points to the function that defines the meaning of the barcodes
extern void (*bcHandlingFunction)(uint8_t);

//using 4 different barcodes
#define BARCODE_NUMBER	4

//allowing one bit error
#define BARCODE_MAX_BIT_ERRORS	1

// barcodes with hamming distance 3 --> 1bit error correction possible
extern const uint8_t BARCODES[4]; // = { 0x2A, 0x06, 0x30, 0x1C }; // {0x15, 0x18, 0x03, 0x0E};

//len of barcodes
extern uint8_t BARCODE_BIT_LENGTH;

//handle to barcode reader thread
extern xTaskHandle xHandleBarcodeReader;

/*
 * initializes barcode reader functionality
 */
void initBarcodeReader(void);

/*
 * starts barcode reader thread
 */
void startBarcodeReader(void);

/*
 * suspends barcode reader thread
 */
void stopBarcodeReader(void);

/*
 * sets the function pointer to a function that handles the meaning of the barcodes
 * @param function pointer (uint8_t) -> void
 */
void setBarcodeHandlingFunction(void(*func)(uint8_t));

/*
 * function that handles detected barcodes
 * @param detected barcode as number
 */
void BarcodeHandlingLeftStraightRightBack(uint8_t barcode);

/*
 * Thread checking for barcodes
 */
void BarcodeReaderThread(void);


#endif /* BARCODE_READER_H_ */
