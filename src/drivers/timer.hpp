#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

void timer_initialize(uint32_t frequency);
void timer_handler();
uint32_t timer_get_ticks();
void timer_wait(uint32_t ticks);

#endif