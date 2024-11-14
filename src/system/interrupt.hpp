#ifndef INTERRUPT_H
#define INTERRUPT_H

#include <stdint.h>

// Interrupt handler type
typedef void (*interrupt_handler_t)();

void register_interrupt_handler(uint8_t interrupt, interrupt_handler_t handler);
void interrupt_enable();
void interrupt_disable();

#endif