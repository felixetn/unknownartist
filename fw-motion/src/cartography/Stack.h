/**
 * (c) Real-time systems project seminar, TU Darmstadt
 *
 * @file Stack.h
 * @author Tobias
 * @brief Implementierung eines Stack zum Speichern von Verzweigungen
 **/

#ifndef STACK_H_
#define STACK_H_


#define STACK_MAX 50

void Stack_Init(void);
boolean_t Stack_isEmpty(void);
int8_t Stack_Top(int16_t* x, int16_t* y, uint16_t* d);
void Stack_Push(int16_t x, int16_t y, uint16_t d);
void Stack_Pop(void);

#endif /* STACK_H_ */
