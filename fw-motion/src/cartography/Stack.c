/**
 * (c) Real-time systems project seminar, TU Darmstadt
 * 
 * @file Stack.c
 * @author Tobias	
 * @brief Implementierung eines Stack zum Speichern von Verzweigungen
 **/

// http://groups.csail.mit.edu/graphics/classes/6.837/F04/cpp_notes/stack1.html

#include <stdio.h>
#include "../api/api.h"
#include "stdlib.h"
#include "Stack.h"


static struct Stack {
		int16_t size;
		int16_t coord_x[STACK_MAX];
		int16_t coord_y[STACK_MAX];
		uint16_t direction[STACK_MAX];
}CrossingStack;


/**
 * Stack Initialisieren
 */
void Stack_Init()
{
    CrossingStack.size = 0;
}

/**
 * @return True wenn leer
 */
boolean_t Stack_isEmpty(){
	if (CrossingStack.size == 0) return TRUE;
	else return FALSE;
}

/**
 * @return oberstes Element vom Stack (jewils x,y,d)
 */
int8_t Stack_Top(int16_t* x, int16_t* y, uint16_t* d)
{
    if (CrossingStack.size == 0) {
        return -1;
    }
    *x = CrossingStack.coord_x[CrossingStack.size];
    *y = CrossingStack.coord_y[CrossingStack.size];
    *d = CrossingStack.direction[CrossingStack.size];

    return 1;
}

/**
 * Wertde dem Stack hinzufügen
 */
void Stack_Push(int16_t x, int16_t y, uint16_t d)
{
    if (CrossingStack.size < STACK_MAX-1){
    	CrossingStack.coord_x[++CrossingStack.size] = x;
    	CrossingStack.coord_y[CrossingStack.size] = y;
    	CrossingStack.direction[CrossingStack.size] = d;
    }


}

/**
 * Oberstes Element löschen
 */
void Stack_Pop()
{
    if (CrossingStack.size > 0)
        CrossingStack.size--;
}
