/******************************************************************************
 /* (C) Fujitsu Microelectronics Europe GmbH                                  */
/*                                                                           */
/* The following software deliverable is intended for and must only be       */
/* used in an evaluation laboratory environment.                             */
/* It is provided without charge and therefore provided on an as-is          */
/* basis, subject to alterations.                                            */
/*                                                                           */
/* The software deliverable is to be used exclusively in connection          */
/* with FME products.                                                        */
/* In the event the software deliverable includes the use of open            */
/* source components, the provisions of the governing open source            */
/* license agreement shall apply with respect to such software               */
/* deliverable.                                                              */
/* FME does not warrant that the deliverables do not infringe any            */
/* third party intellectual property right (IPR). In the event that          */
/* the deliverables infringe a third party IPR it is the sole                */
/* responsibility of the customer to obtain necessary licenses to            */
/* continue the usage of the deliverable.                                    */
/*                                                                           */
/* To the maximum extent permitted by applicable law FME disclaims all       */
/* warranties, whether express or implied, in particular, but not            */
/* limited to, warranties of merchantability and fitness for a               */
/* particular purpose for which the deliverable is not designated.           */
/*                                                                           */
/* To the maximum extent permitted by applicable law, FME’s liability        */
/* is restricted to intention and gross negligence.                          */
/* FME is not liable for consequential damages.                              */
/*                                                                           */
/* (Disclaimer V1.2)                                                         */
/*****************************************************************************/
/** \file seg.h
 **
 ** Routines for numerical output on seven segment displays
 **
 ** History:
 **   - 2009-12-17  1.00  CIl  First version
 *****************************************************************************/

#ifndef SEG_H_
#define SEG_H_

/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/
#include "base_type.h"

/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define')                           */
/*****************************************************************************/

/*****************************************************************************/
/* Global type definitions ('typedef')                                       */
/*****************************************************************************/

/*****************************************************************************/
/* Global variable declarations ('extern', definition in C source)           */
/*****************************************************************************/

/*****************************************************************************/
/* Global function prototypes ('extern', definition in C source)             */
/*****************************************************************************/

/**
 * Initializes pins used by seven segment display
 */
void Seg_Init(void);

/**
 * Outputs a hex digit
 * @param hex Value (0..15) to output
 */
void Seg_Hex(uint8_t hex);

/**
 * Outputs a decimal digit
 * @param num Value (0..9) to output
 */
void Seg_Dec(uint8_t num);

#endif /* SEG_H_ */
