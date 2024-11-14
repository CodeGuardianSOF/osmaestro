#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "process.hpp"

void scheduler_init();
void scheduler_add(process* proc);
void scheduler_remove(process* proc);
void scheduler_run();

#endif