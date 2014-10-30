/**
 * (c) Real-time systems project seminar, TU Darmstadt
 *
 * @file sdcard.h
 * @author pnotz
 * @brief provides functions to access SD card on a file system level
 * e. g. logging data to file or reading / writing some data from/to a file
 **/

#ifndef SDCARD_H_
#define SDCARD_H_


#include "fat/ff.h"
#include "../api/api.h"
#include "../api/seg.h"
#include "../api/printf_stdarg.h"
#include "../cartography/initValues.h"
#include "mmc.h"

//indicator if card is initialized: 0: not initialized
extern int sdcInitDone;

/*
 * initializes sd card and reads config file
 */
void initSDCard(void);

/*
 * opens a file for data logging
 */
void initLogFile(void);

/*
 * sets filename according to file ID
 * -1: configuration, carconf.dat
 * else: f%d.dat
 */
void SetCurrentFileID(int fileID, int type);

/*
 * check if log file is ready to be written to
 */
int LogReady(void);

/*
 * write data to logfile
 */
void LogDataToFile(char* Buffer, unsigned int len);

/*
 * synchronizes log file - buffer is flushed and written to sd card
 */
void syncLogFile(void);

/*
 * closes log file
 */
void closeLogFile(void);

/*
 * reads data from a file into a given buffer
 * type == 1 = Standard
 */
int ReadFromFile(int fileID, char* Buffer, unsigned int start, unsigned int len, int type);

/*
 * writes data from a given buffer to a file on the sd card
 */
int WriteToFile(int fileID, char* Buffer, unsigned int len, int type);

int FileExists(int fileID);
void close_file(void);
void delete_file(int fileID, int type);

#endif /* SDCARD_H_ */
