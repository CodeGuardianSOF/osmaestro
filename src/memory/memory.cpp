#include "memory.hpp"

// Memory management variables
static memory_block* free_list = nullptr;
static const size_t HEAP_SIZE = 1024 * 1024; // 1MB heap
static uint8_t heap[HEAP_SIZE];

void memory_initialize() {
    // Initialize the first free block
    free_list = (memory_block*)heap;
    free_list->size = HEAP_SIZE - sizeof(memory_block);
    free_list->is_free = 1;
    free_list->next = nullptr;
}

void* malloc(size_t size) {
    memory_block* current = free_list;
    memory_block* prev = nullptr;
    
    while(current != nullptr) {
        if(current->is_free && current->size >= size) {
            if(current->size > size + sizeof(memory_block)) {
                // Split block
                memory_block* new_block = (memory_block*)((uint8_t*)current + sizeof(memory_block) + size);
                new_block->size = current->size - size - sizeof(memory_block);
                new_block->is_free = 1;
                new_block->next = current->next;
                
                current->size = size;
                current->next = new_block;
            }
            
            current->is_free = 0;
            return (void*)((uint8_t*)current + sizeof(memory_block));
        }
        
        prev = current;
        current = current->next;
    }
    
    return nullptr; // No free memory
}

void free(void* ptr) {
    if(!ptr) return;
    
    memory_block* block = (memory_block*)((uint8_t*)ptr - sizeof(memory_block));
    block->is_free = 1;
    
    // Merge with next block if it's free
    if(block->next && block->next->is_free) {
        block->size += sizeof(memory_block) + block->next->size;
        block->next = block->next->next;
    }
}

void* memset(void* ptr, int value, size_t num) {
    uint8_t* p = (uint8_t*)ptr;
    while(num--) *p++ = (uint8_t)value;
    return ptr;
}

void* memcpy(void* destination, const void* source, size_t num) {
    uint8_t* dest = (uint8_t*)destination;
    const uint8_t* src = (const uint8_t*)source;
    while(num--) *dest++ = *src++;
    return destination;
}