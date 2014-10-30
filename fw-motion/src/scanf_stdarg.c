/*
 * scanf_stdarg.c
 *
 *  Created on: 01.07.2010
 *  Author: prosem
 */
#include <stdarg.h>
#include "scanf_stdarg.h"
#include "api/serial.h"

static int isQuotion(char token) {
	return token == '"';
}

static int isWhitespace(char token) {
	return token == ' ' || token == '\t' || token == '\r' || token == '\n';
}

int scanWhitespaces(void) {
	char token = 0;
	int whitespaces = 0;
	while (isWhitespace(token)) {
		whitespaces++;
		token = Serial_Getchar();
		//Write immediatly everything to the terminal which was typed by the user
		Serial_PutChar(token);
	}
	return whitespaces;
}

char * scanWord(char * result) {

	char token;
	int resultIndex = 0;
	int quoteStarted = 0;
	token = Serial_Getchar();

	//Skip all trailing whitespaces
	while (isWhitespace(token)) {
		token = Serial_Getchar();
		//Write immediatly everything to the terminal which was typed by the user
		Serial_PutChar(token);
	}

	//If a quoted string starts, we have to wait until it ends
	if (isQuotion(token)) {
		quoteStarted = 1;
		//Skip it
		token = Serial_Getchar();
		//Write immediatly everything to the terminal which was typed by the user
		Serial_PutChar(token);
	}

	while (((!isWhitespace(token)) && (!quoteStarted) && (!isQuotion(token)))
			|| (quoteStarted && (!isQuotion(token)))) {
		result[resultIndex++] = token;
		token = Serial_Getchar();
		//Write immediatly everything to the terminal which was typed by the user
		Serial_PutChar(token);
	}
	result[resultIndex++] = '\0';

	return 0;
}
