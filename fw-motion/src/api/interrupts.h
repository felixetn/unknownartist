#ifndef INTERRUPTS_H_
#define INTERRUPTS_H_

#include "base_type.h"

typedef void __interrupt (* IRQHandler)(void);

/**
 * Initialize memory stored interrupt table
 */
extern void interrupts_init(void);

/**
 * Registers an interrupt handler method
 * \param interruptNr Interrupt number to handle
 * \param interruptLevel Priority for this interrupt (0 = highest, 6 = lowest, 7 = off)
 * \param handler Pointer to the interrupt handler
 */
extern void interrupts_setHandler(uint8_t interruptNr, uint8_t interruptLevel, IRQHandler handler);

/**
 * Disables the given interrupt
 * \param interruptNr Interrupt number of the interrupt to disable
 */
extern void interrupts_disableInterrupt(uint8_t interruptNr);

/**
 * Set the lowest interrupt level to be allowed for execution
 */
#define interrupts_setLevel(maxLevel)  __set_il(maxLevel)

/**
 * Enable interrupt execution
 */
#define interrupts_enable()   __EI()

/**
 * Disable interrupt execution
 */
#define interrupts_disable()   __DI()

#endif /* INTERRUPTS_H_ */
