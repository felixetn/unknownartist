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

/************************************************************************
 *  Module:       tal_defs.h
 *  Description:  basic type definitions
 *
 *  Author(s):    Steffen Weiß
 *  Company:      Thesycon GmbH, Ilmenau
 ************************************************************************/
#ifndef tal_defs_h_included
#define tal_defs_h_included


/*
  define basic integer types of known sizes,
  you have to adjust it for your environment
*/

/* boolean */
typedef unsigned int    UINT;

/*  8 bit unsigned */
typedef unsigned char   UCHAR;

/* 16 bit unsigned */
typedef unsigned short  USHORT;

/* 16 bit signed */
typedef signed short    SHORT;

/* 32 bit unsigned */
typedef unsigned long   ULONG;

/* 32 bit   signed */
typedef signed long     LONG;


/* compiler specific */
#define  BOOL UCHAR


/*
  common defines and macros used in the adaption layer,
  !!! do not change !!!
*/

#ifndef NULL
#if defined(__cplusplus)
#define NULL (0)
#else
#define NULL ((void*)0)
#endif
#endif

#ifndef FALSE
#define FALSE (0)
#endif

#ifndef TRUE
#define TRUE  (1)
#endif

#define MSB(word)      (T_UINT8)(((T_UINT16)(word) >> 8) & 0xff)
#define LSB(word)      ((T_UINT8)(word))



#endif  /* tal_defs_h_included */

/********************************* EOF **********************************/
