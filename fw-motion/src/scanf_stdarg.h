/*
 * scanf_stdarg.h
 *
 *  Created on: 01.07.2010
 *      Author: prosem
 */

#ifndef SCANF_STDARG_H_
#define SCANF_STDARG_H_
/***
 * Reads all white space characters form the standart input
 */
extern int scanWhitespaces(void);

/***
 * Reads the next word from the standart input. A
 * word is separated from the next characters by a
 * whitespace.
 *
 * @param buffer is the character array wich is filled
 * with the data
 */
extern char * scanWord(char* buffer);

#endif /* SCANF_STDARG_H_ */
