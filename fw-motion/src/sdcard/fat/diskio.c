/**
 * (c) Real-time systems project seminar, TU Darmstadt
 * 
 **/

/*
==========================================================================

The files
- diskio.c
- diskio.h
- ff.c
- ff.h
- integer.h
- tal_defs.h
of the FAT16 file system are provided by the Fujitsu project:
"USB-Host Project for SK-16FX-144PMC-USB Evaluation Board"

The project is based on software by ELM-ChaN which can be downloaded at:
http://elm-chan.org/fsw/ff/00index_e.html

The code was modified to fit into this project.

 */

/*-----------------------------------------------------------------------
/  Low level disk interface modlue include file  R0.05   (C)ChaN, 2007
/-----------------------------------------------------------------------*/

#include "diskio.h"

/*-----------------------------------------------------------------------*/
/* Correspondence between drive number and physical drive                */
/* Note that Tiny-FatFs supports only single drive and always            */
/* accesses drive number 0.                                              */


/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */

DSTATUS disk_initialize (
	BYTE drv				/* Physical drive nmuber (0..) */
)
{
	DSTATUS stat;

	stat=0;

	return stat;
}



/*-----------------------------------------------------------------------*/
/* Return Disk Status                                                    */

DSTATUS disk_status (
	BYTE drv		/* Physical drive nmuber (0..) */
)
{
	DSTATUS stat;

	stat=0; //not protected
	return stat;

}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */

DRESULT disk_read (
	BYTE drv,		/* Physical drive nmuber (0..) */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector number (LBA) */
	BYTE count		/* Sector count (1..255) */
)
{
	DRESULT res;
	int result;
	int j=0;

	for(j=0;j<count;j++)
	{
		mmc_read_sector(sector+j, buff + 512*j);
	}

	return RES_OK;

}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */

#if _READONLY == 0
DRESULT disk_write (
	BYTE drv,			/* Physical drive nmuber (0..) */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector number (LBA) */
	BYTE count			/* Sector count (1..255) */
)
{
	DRESULT res;
	int result;
	int j=0;

	for(j=0;j<count;j++)
	{
		mmc_write_sector(sector+j, buff + 512*j);
	}
	return RES_OK;

}
#endif /* _READONLY */



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */

DRESULT disk_ioctl (
	BYTE drv,		/* Physical drive nmuber (0..) */
	BYTE ctrl,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	if(ctrl == CTRL_SYNC) //sync - nothing to do
		return RES_OK;

	return RES_PARERR;
}


//Get FATTIME DUMMY

DWORD get_fattime (void){
//return 0x2210821; //01.01.1981 01:01
	return 0x40B7BAE0; //23.05.2012 23:23:00
}

