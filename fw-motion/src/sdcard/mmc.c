/**
 * (c) Real-time systems project seminar, TU Darmstadt
 *
 * @file mmc.c
 * @author Modified by pnotz
 **/

/*#######################################################################################
 Connect AVR to MMC/SD

 Copyright (C) 2004 Ulrich Radig

 Bei Fragen und Verbesserungen wendet euch per EMail an

 mail@ulrichradig.de

 oder im Forum meiner Web Page : www.ulrichradig.de

 Dieses Programm ist freie Software. Sie können es unter den Bedingungen der
 GNU General Public License, wie von der Free Software Foundation veröffentlicht,
 weitergeben und/oder modifizieren, entweder gemäß Version 2 der Lizenz oder
 (nach Ihrer Option) jeder späteren Version.

 Die Veröffentlichung dieses Programms erfolgt in der Hoffnung,
 daß es Ihnen von Nutzen sein wird, aber OHNE IRGENDEINE GARANTIE,
 sogar ohne die implizite Garantie der MARKTREIFE oder der VERWENDBARKEIT
 FÜR EINEN BESTIMMTEN ZWECK. Details finden Sie in der GNU General Public License.

 Sie sollten eine Kopie der GNU General Public License zusammen mit diesem
 Programm erhalten haben.
 Falls nicht, schreiben Sie an die Free Software Foundation,
 Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.
 #######################################################################################*/

#include "mmc.h"
#include "../api/api.h"

int pinFactor = 3;

void MMC_Disable() {
	mmc_setCS(1);
}

void MMC_Enable() {
	mmc_setCS(0);
}

void mmc_setCS(int val) {
	uint8_t i = 0;

	for (i = 0; i < pinFactor; i++) {
		PDR00_P7 = val;
	}
}

void mmc_setCLK(int val) {
	uint8_t i = 0;

	for (i = 0; i < pinFactor; i++) {
		PDR06_P7 = val;
	}
}

void mmc_setDI(int val) {
	uint8_t i = 0;

	for (i = 0; i < pinFactor; i++) {
		PDR06_P5 = val;
	}
}

int mmc_getDO() {
	return PDR07_P3;
}

int mmc_getCardStatus() {
	return PDR07_P5;
}

void mmc_initPorts() {
	//init data direction registers
	DDR06_D7 = 1;
	DDR06_D5 = 1;
	DDR07_D5 = 0;
	DDR07_D3 = 0;
	DDR00_D7 = 1;

	//enable input pins
	PIER07_IE3 = 1;
	PIER07_IE5 = 1;

	//set input port levels: TTL
	PILR07_IL3 = 0; //0
	EPILR07_EIL3 = 1; //1
	PILR07_IL5 = 0; //0
	EPILR07_EIL5 = 1; //1

	//setting reduced current output drive
	PODR06_OD7 = 1;
	PODR06_OD5 = 1;
	PODR00_OD7 = 1;
}

//############################################################################
//Routine zur Initialisierung der MMC/SD-Karte (SPI-MODE)
unsigned char mmc_init()
//############################################################################
{
	unsigned int Timeout = 0;
	unsigned int a = 0;
	unsigned int b = 0;
	unsigned char CMD[] = { 0x40, 0x00, 0x00, 0x00, 0x00, 0x95 };

	mmc_initPorts();

	MMC_Disable(); //Setzt den Pin MMC_Chip_Select auf High Pegel

	os_wait(500);

	//Initialisiere MMC/SD-Karte in den SPI-Mode
	for (b = 0; b < 0x0f; b++) //Sendet min 74+ Clocks an die MMC/SD-Karte
			{
		mmc_write_byte(0xff);
	}

	//Sendet Commando CMD0 an MMC/SD-Karte
	while (mmc_write_command(CMD) != 1) {
		if (Timeout++ > 200) {
			MMC_Disable();
			return (1); //Abbruch bei Commando1 (Return Code1)
		}
	}
	//Sendet Commando CMD1 an MMC/SD-Karte
	Timeout = 0;
	CMD[0] = 0x41; //Commando 1
	CMD[5] = 0xFF;
	while (mmc_write_command(CMD) != 0) {
		if (Timeout++ > 400) {
			MMC_Disable();
			return (2); //Abbruch bei Commando2 (Return Code2)
		}
	}

	//set MMC_Chip_Select to high (MMC/SD-Karte Inaktiv)
	MMC_Disable();
	return (0);
}

//############################################################################
//Sendet ein Commando an die MMC/SD-Karte
unsigned char mmc_write_command(unsigned char *cmd)
//############################################################################
{
	unsigned char tmp = 0xff;
	unsigned int Timeout = 0;
	unsigned int a = 0;

	//set MMC_Chip_Select to high (MMC/SD-Karte Inaktiv)
	MMC_Disable();

	//sendet 8 Clock Impulse
	mmc_write_byte(0xFF);

	//set MMC_Chip_Select to low (MMC/SD-Karte Aktiv)
	MMC_Enable();

	//sendet 6 Byte Commando
	for (a = 0; a < 0x06; a++) //sendet 6 Byte Commando zur MMC/SD-Karte
			{
		mmc_write_byte(*cmd++);
	}

	//Wartet auf ein gültige Antwort von der MMC/SD-Karte
	while (tmp == 0xff) {
		tmp = mmc_read_byte();
		if (Timeout++ > 500) {
			break; //Abbruch da die MMC/SD-Karte nicht Antwortet
		}
	}
	return (tmp);
}

//############################################################################
//Routine zum Empfangen eines Bytes von der MMC-Karte
unsigned char mmc_read_byte(void)
//############################################################################
{
	unsigned char Byte = 0;

	unsigned char a = 8;
	for (a = 8; a > 0; a--) //das Byte wird Bitweise nacheinander Empangen MSB First
			{
		//MMC_Write &=~(1<<SPI_Clock); //erzeugt ein Clock Impuls (Low)
		mmc_setCLK(0); //clock to 0

		if (mmc_getDO() > 0) //bit_is_set(MMC_Read,SPI_DI) > 0) //Lesen des Pegels von MMC_DI
				{
			Byte |= (1 << (a - 1));
		} else {
			Byte &= ~(1 << (a - 1));
		}
		//MMC_Write |=(1<<SPI_Clock); //setzt Clock Impuls wieder auf (High)
		mmc_setCLK(1); //clock to 1

	}
	return (Byte);
}

//############################################################################
//Routine zum Senden eines Bytes zur MMC-Karte
void mmc_write_byte(unsigned char Byte)
//############################################################################
{

	unsigned char a = 8;
	unsigned int i = 0; //waitvar
	unsigned int j = 0;

	for (a = 8; a > 0; a--) //das Byte wird Bitweise nacheinander Gesendet MSB First
			{
		if ((Byte & (1 << (a - 1))) > 0) //Ist Bit a in Byte gesetzt
				{
			//MMC_Write |= (1<<SPI_DO); //Set Output High
			mmc_setDI(1);
		} else {
			//MMC_Write &= ~(1<<SPI_DO); //Set Output Low
			mmc_setDI(0);
		}
		//MMC_Write &= ~(1<<SPI_Clock);	//erzeugt ein Clock Impuls (LOW)
		mmc_setCLK(0);

		//MMC_Write |= (1<<SPI_Clock); //setzt Clock Impuls wieder auf (High)
		mmc_setCLK(1);

	}
	//MMC_Write |= (1<<SPI_DO);	//setzt Output wieder auf High
	mmc_setDI(1);

}

//############################################################################
//Routine zum schreiben eines Blocks(512Byte) auf die MMC/SD-Karte
unsigned char mmc_write_sector(unsigned long addr, unsigned char *Buffer)
//############################################################################
{
	unsigned char tmp;
	unsigned int a = 0;
	//Commando 24 zum schreiben eines Blocks auf die MMC/SD - Karte
	unsigned char cmd[] = { 0x58, 0x00, 0x00, 0x00, 0x00, 0xFF };

	/*Die Adressierung der MMC/SD-Karte wird in Bytes angegeben,
	 addr wird von Blocks zu Bytes umgerechnet danach werden
	 diese in das Commando eingefügt*/

	addr = addr << 9; //addr = addr * 512

	cmd[1] = ((addr & 0xFF000000) >> 24 );
	cmd[2] = ((addr & 0x00FF0000) >> 16 );
	cmd[3] = ((addr & 0x0000FF00) >> 8 );

	//Sendet Commando cmd24 an MMC/SD-Karte (Write 1 Block/512 Bytes)
	tmp = mmc_write_command(cmd);
	if (tmp != 0) {
		return (tmp);
	}

	//Wartet einen Moment und sendet einen Clock an die MMC/SD-Karte
	for (a = 0; a < 100; a++) {
		mmc_read_byte();
	}

	//Sendet Start Byte an MMC/SD-Karte
	mmc_write_byte(0xFE);

	//Schreiben des Bolcks (512Bytes) auf MMC/SD-Karte
	for (a = 0; a < 512; a++) {
		mmc_write_byte(*Buffer++);
	}

	//CRC-Byte schreiben
	mmc_write_byte(0xFF); //Schreibt Dummy CRC
	mmc_write_byte(0xFF); //CRC Code wird nicht benutzt

	//Fehler beim schreiben? (Data Response XXX00101 = OK)
	if ((mmc_read_byte() & 0x1F) != 0x05)
		return (1);

	//Wartet auf MMC/SD-Karte Bussy
	while (mmc_read_byte() != 0xff) {
	};

	//set MMC_Chip_Select to high (MMC/SD-Karte Inaktiv)
	MMC_Disable();

	return (0);
}

//############################################################################
//Routine zum lesen des CID Registers von der MMC/SD-Karte (16Bytes)
void mmc_read_block(unsigned char *cmd, unsigned char *Buffer,
		unsigned int Bytes)
//############################################################################
{
	unsigned int a = 0;

	//Sendet Commando cmd an MMC/SD-Karte
	if (mmc_write_command(cmd) != 0) {
		return;
	}

	//Wartet auf Start Byte von der MMC/SD-Karte (FEh/Start Byte)

	while (mmc_read_byte() != 0xfe) {
	};

	//Lesen des Blocks (normal 512Bytes) von MMC/SD-Karte
	for (a = 0; a < Bytes; a++) {
		*Buffer++ = mmc_read_byte();
	}
	//CRC-Byte auslesen
	mmc_read_byte(); //CRC - Byte wird nicht ausgewertet
	mmc_read_byte(); //CRC - Byte wird nicht ausgewertet

	//set MMC_Chip_Select to high (MMC/SD-Karte Inaktiv)
	MMC_Disable();

	return;
}

//############################################################################
//Routine zum lesen eines Blocks(512Byte) von der MMC/SD-Karte
unsigned char mmc_read_sector(unsigned long addr, unsigned char *Buffer)
//############################################################################
{
	//Commando 16 zum lesen eines Blocks von der MMC/SD - Karte
	unsigned char cmd[] = { 0x51, 0x00, 0x00, 0x00, 0x00, 0xFF };

	/*Die Adressierung der MMC/SD-Karte wird in Bytes angegeben,
	 addr wird von Blocks zu Bytes umgerechnet danach werden
	 diese in das Commando eingefügt*/

	addr = addr << 9; //addr = addr * 512

	cmd[1] = ((addr & 0xFF000000) >> 24 );
	cmd[2] = ((addr & 0x00FF0000) >> 16 );
	cmd[3] = ((addr & 0x0000FF00) >> 8 );

	mmc_read_block(cmd, Buffer, 512);

	return (0);
}
