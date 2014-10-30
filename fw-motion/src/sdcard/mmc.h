/**
 * (c) Real-time systems project seminar, TU Darmstadt
 * 
 * @file mmc.h
 * @author Modified by pnotz
 * @brief providing low level functions to communicate with SD card
 * (same interface as MMC)
 **/

#ifndef MMC_H_
#define MMC_H_



/*#######################################################################################
Connect ARM to MMC/SD

Copyright (C) 2004 Ulrich Radig
#######################################################################################*/

#include "../api/base_type.h"

//Prototypes
extern unsigned char mmc_read_byte(void);

extern void mmc_write_byte(unsigned char);

extern void mmc_read_block(unsigned char *,unsigned char *,unsigned in);

extern unsigned char mmc_init(void);

extern unsigned char mmc_read_sector (unsigned long,unsigned char *);

extern unsigned char mmc_write_sector (unsigned long,unsigned char *);

extern unsigned char mmc_write_command (unsigned char *);


extern void mmc_setCLK(int val);
extern void mmc_setDI(int val);
extern void mmc_setCS(int val);
extern int mmc_getDO(void);
extern int mmc_getCardStatus(void);
extern void mmc_initPorts(void);
extern void MMC_Disable(void); //set MMC_Chip_Select to high (MMC/SD-Karte Inaktiv)
extern void MMC_Enable(void); //set MMC_Chip_Select to low (MMC/SD-Karte Aktiv)

#endif /* MMC_H_ */
