#ifndef SERIAL_H
#define SERIAL_H

#include "FreeRTOS.h"
#include "base_type.h"

char Serial_Getchar(void);

/***
 * Try to fetch the next char from the standard in if
 * there is an input present within the blockTimeInMs
 */
int16_t Serial_GetcharAsync(int blockTimeInMs);

/***
 * Writes the given char to the serial port which is defined as default output port
 *
 * @param Char The character to send.
 */
void Serial_PutChar(char Char);

/***
 * Initializes the standard out.
 */
void Serial_Init(void);
#endif

