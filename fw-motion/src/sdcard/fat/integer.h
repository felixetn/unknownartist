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

#ifndef _INTEGER

/******************************************************************
   Following typedefs were partly commented due to conmflict with
   type defs in "tal_defs.h"
 ******************************************************************

// These types are assumed as 16-bit or larger integer
*/
typedef signed int		INT;
/*
typedef unsigned int	UINT;

// These types are assumed as 8-bit integer
typedef signed char		CHAR;
typedef unsigned char	UCHAR;
*/
typedef unsigned char	BYTE;
/*
// These types are assumed as 16-bit integer
typedef signed short	SHORT;
typedef unsigned short	USHORT;
*/
typedef unsigned short	WORD;
/*
// These types are assumed as 32-bit integer
typedef signed long		LONG;
typedef unsigned long	ULONG;
*/
typedef unsigned long	DWORD;



/* Boolean type */
typedef enum { FALSEr=0, TRUEr } BOOL;

//#define FALSE 0
//#define TRUE 1

#define _INTEGER
#endif
