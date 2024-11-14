#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>

// Process states
enum process_state {
    PROCESS_READY,
    PROCESS_RUNNING,
    PROCESS_BLOCKED,
    PROCESS_TERMINATED
};

// Process control block
struct process {
    uint32_t pid;
    process_state state;
    uint32_t* stack;
    uint32_t stack_size;
    struct process* next;
};

void process_init();
process* process_create(void (*entry)());
void process_terminate(process* proc);
void process_check();

#endif