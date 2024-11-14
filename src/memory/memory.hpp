#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>
#include <stddef.h>

void memory_initialize();
void* malloc(size_t size);
void free(void* ptr);
void* memset(void* ptr, int value, size_t num);
void* memcpy(void* destination, const void* source, size_t num);

// Memory management structures for x64
struct memory_block {
    size_t size;
    uint8_t is_free;
    struct memory_block* next;
    uint64_t padding; // Ensure 64-bit alignment
};

// Physical Memory Manager
class PhysicalMemoryManager {
public:
    static void initialize();
    static void* allocate_frame();
    static void free_frame(void* frame);
    
private:
    static uint64_t* bitmap;
    static size_t bitmap_size;
};

// Virtual Memory Manager
class VirtualMemoryManager {
public:
    static void initialize();
    static void map_page(uint64_t virtual_addr, uint64_t physical_addr);
    static void unmap_page(uint64_t virtual_addr);
    
private:
    static uint64_t* get_pml4();
    static void create_page_tables();
};

#endif