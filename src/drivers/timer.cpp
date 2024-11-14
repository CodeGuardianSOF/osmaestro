#include "timer.hpp"
#include "../io/ports.hpp"
#include "../system/interrupt.hpp"

static uint32_t tick = 0;

void timer_initialize(uint32_t frequency) {
    uint32_t divisor = 1193180 / frequency;
    
    outb(0x43, 0x36);
    outb(0x40, divisor & 0xFF);
    outb(0x40, (divisor >> 8) & 0xFF);
    
    register_interrupt_handler(32, timer_handler);
}

void timer_handler() {
    tick++;
}

uint32_t timer_get_ticks() {
    return tick;
}

void timer_wait(uint32_t ticks) {
    uint32_t target = tick + ticks;
    while(tick < target);
}