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
/** \file seg.c
 **
 ** Routines for numerical output on seven segment displays
 **
 ** History:
 **   - 2009-12-17  1.00  CIl  First version
 *****************************************************************************/

/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/
#include "seg.h"

/*****************************************************************************/
/* Local pre-processor symbols/macros ('#define')                            */
/*****************************************************************************/
#define SEGMASK 0x7f
#define UC(x) (uint8_t)(x)

/*****************************************************************************/
/* Global variable definitions (declared in header file with 'extern')       */
/*****************************************************************************/

/*****************************************************************************/
/* Local type definitions ('typedef')                                        */
/*****************************************************************************/

/*****************************************************************************/
/* Local variable definitions ('static')                                     */
/*****************************************************************************/
static uint8_t DEC7SEG[16] = { UC(~0x3f), UC(~0x06), UC(~0x5b), UC(~0x4f),
        UC(~0x66), UC(~0x6d), UC(~0x7d), UC(~0x07), UC(~0x7f), UC(~0x6f),
        UC(~0x77), UC(~0x7c), UC(~0x58), UC(~0x5e), UC(~0x79), UC(~0x71) };

/*****************************************************************************/
/* Local function prototypes ('static')                                      */
/*****************************************************************************/

/*****************************************************************************/
/* Function implementation - global ('extern') and local ('static')          */
/*****************************************************************************/

void Seg_Init(void)
{
    PDR09 = 0xff;
    DDR09 = 0xff;
}

void Seg_Hex(uint8_t hex)
{
    if (hex < 16)
    {
        PDR09 |= SEGMASK;
        PDR09 = DEC7SEG[hex];
    }
}

void Seg_Dec(uint8_t num)
{
    if (num < 10)
    {
        PDR09 |= SEGMASK;
        PDR09 = DEC7SEG[num];
    }
}
