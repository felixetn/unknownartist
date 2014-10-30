/**
 * (c) Real-time systems project seminar, TU Darmstadt
 *
 * @file sdcard.c
 * @author pnotz
 * @brief provides functions to access SD card on a file system level
 * e. g. logging data to file or reading / writing some data from/to a file
 **/

#include "sdcard.h"
#include "../api/wireless/wireless.h"

int curFileID=-99; //number of file that is currently used - for example for data logging
char sdcFilename[12]; //filename storage, 8.3 format
int sdcLogFileOpen=0; //switch if logfile is open

FATFS fatfs[1];
FIL ffile; //file pointer for logfile
static FIL ffile1; //file pointer for reading/writing file

int sdcInitDone = 0;

/*
 * initializes sd card and reads config file
 */
void initSDCard()
{
	int retVal=0;
	int timeout = 0;
	DWORD P1;

//	if(sdcInitDone) //init procedure already started
//		return; // Thread wird sowieso beendet

	curFileID=-99;

	//Set output Pins for LEDs
	DDR03_D7 = 1; //Read (Pin 16)
	DDR03_D6 = 1; //Write (Pin 15)
	PDR03_P7 = 1;
	PDR03_P6 = 1;


	mmc_initPorts();

//	Seg_Hex(0xC);

	//wait for card to be inserted
	while(mmc_getCardStatus() > 0) //it is 0 if we have a card
	{
		//return after 10 sec
		if(timeout > 20)
			return;
#ifdef SDCARD_DEBUG
		printf("waiting for card.. \r\n");
#endif
		os_wait(500);
		timeout++;
	}

//	Seg_Hex(0xF);

	os_wait(200);

	//initialize card
	retVal=mmc_init();

	os_wait(200);

	//mount FAT file system
	P1=0;
	f_mount((BYTE)P1, &fatfs[P1]);

	if(curFileID==-99) //has not been set
		SetCurrentFileID(1,1);

	sdcInitDone=1;
	//read configuration file from sd card
	readConfigFromFile();
	Seg_Hex(0x00);
}

/*
 * opens a file for data logging
 */
void initLogFile()
{

	if(!sdcInitDone)
		return;

	//update filename config to current file
	SetCurrentFileID(curFileID,1);

	f_open(&ffile,(char*)sdcFilename, FA_OPEN_ALWAYS | FA_WRITE); //FA_OPEN_EXISTING

	if(ffile.fsize>0) //file did exist before
	{
		f_lseek(&ffile, ffile.fsize); //move pointer to the end of file
	}

	syncLogFile();

	//log file is opened and it can be written to it
	sdcLogFileOpen=1;

}

/*
 * check if log file is ready to be written to
 */
int LogReady()
{
	return sdcLogFileOpen;
}

/*
 * closes log file
 */
void closeLogFile()
{
	//check if log file is open
	if(!sdcLogFileOpen)
		return;

	f_close(&ffile);

	sdcLogFileOpen=0;
}

/*
 * synchronizes log file - buffer is flushed and written to sd card
 */
void syncLogFile()
{
	if(!sdcLogFileOpen)
		return;

	f_sync(&ffile);
}

/*
 * sets filename according to file ID
 * -1: configuration, carconf.dat
 * else: f%d.dat
 */
void SetCurrentFileID(int fileID, int type)
{
	if(fileID==-1) //config
	{
		sprintf(sdcFilename, "carconf.dat");
	}
	else
	{
		curFileID=fileID;
		if (type == 1)
			sprintf(sdcFilename, "f%d.dat", curFileID);
		else sprintf(sdcFilename, "m%d.dat", curFileID);
	}
}

/*
 * write data to logfile
 */
void LogDataToFile(char* Buffer, unsigned int len)
{
	UINT s2;

	if(!sdcLogFileOpen)
		return;

	f_write(&ffile,(char*) Buffer,len,&s2);

}

/*
 * writes data from a given buffer to a file on the sd card
 */
int WriteToFile(int fileID, char* Buffer, unsigned int len, int type)
{
	UINT s2;
	FRESULT res;
	int oldFileID=curFileID; //save current file id

	//Ausgabe de Zugriffs auf PIN 16
	PDR03_P6 = 0;
	//check if sd card is initialized
	if(!sdcInitDone)
		return -1;
	res = 1;
	//set current file per file ID
	SetCurrentFileID(fileID, type);
		res = f_open(&ffile1,(char*)sdcFilename, FA_OPEN_ALWAYS | FA_WRITE);

	if (res == FR_OK){
		res = f_write(&ffile1,(char*) Buffer,len,&s2);
		if (res != FR_OK){
			Seg_Hex(0x0D);
		}
	}
	else{
		Seg_Hex(0x0C);
	}
	//OK , INVALID_OBJECT, NOT_READY, FR_RW_ERROR

		f_close(&ffile1);
	//set filename config back to original value
	SetCurrentFileID(oldFileID, type);
	PDR03_P6 = 1;

	return res;
}

/*
 * reads data from a file into a given buffer
 */
int ReadFromFile(int fileID, char* Buffer, unsigned int start, unsigned int len, int type)
{
	FRESULT res;
	UINT ByteRead =0;
	int oldFileID=curFileID; //save current file id

	//Ausgabe de Zugriffs auf PIN 15
	PDR03_P7 = 0;

	//check if sd card is initialized
	if(!sdcInitDone)
		return -1;

	//set current file per file ID
	SetCurrentFileID(fileID, type);

	res = f_open(&ffile1,(char*)sdcFilename, FA_READ);
	if (res != FR_OK){
	}
	else{
		if(start>0)
		{
			f_lseek(&ffile1, start);
		}

		res = f_read(&ffile1, Buffer, (UINT)len, &ByteRead);
		if (res != FR_OK) Seg_Hex(0x0F);
	}

	f_close(&ffile1);

	//set filename config back to original value
	SetCurrentFileID(oldFileID, type);

	PDR03_P7 = 1;

	return res;
}

/*
 * @brief Überprüfen ob eine Datei bereits existiert
 * @return 0 = FileExists
 */
int FileExists(int fileID){
	FRESULT res;
	//check if sd card is initialized
	if(!sdcInitDone)
		return -1;

	SetCurrentFileID(fileID, 1);

	res = f_open(&ffile1,(char*)sdcFilename, FA_READ | FA_WRITE);

		f_close(&ffile1);

	return res;
}

/**
 * @brief Datei manuell schließen
 */
void close_file(){
	f_close(&ffile1);
}

/**
 * @brief einzelen Datei löschen
 * @param Name der Datei
 * @param Dateityo (1 oder 2)
 */
void delete_file(int fileID, int type){

	SetCurrentFileID(fileID, type);
	if(FileExists(fileID) == 1) f_unlink((char*) sdcFilename);


}
