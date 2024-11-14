#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>
#include <stddef.h>

void memory_initialize();
void* malloc(size_t size);
void free(void* ptr);
void* memset(void* ptr, int value, size_t num);
void* memcpy(void* destination, const void* source, size_t num);

// Memory management structures
struct memory_block {
    size_t size;
    uint8_t is_free;
    struct memory_block* next;
};

#endif