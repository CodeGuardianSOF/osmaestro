#include "memory.hpp"

// Memory management variables
static memory_block* free_list = nullptr;
static const size_t HEAP_SIZE = 1024 * 1024 * 1024; // 1GB heap for x64
static uint8_t heap[HEAP_SIZE] __attribute__((aligned(4096)));

uint64_t* PhysicalMemoryManager::bitmap = nullptr;
size_t PhysicalMemoryManager::bitmap_size = 0;

void memory_initialize() {
    // Initialize the first free block
    free_list = (memory_block*)heap;
    free_list->size = HEAP_SIZE - sizeof(memory_block);
    free_list->is_free = 1;
    free_list->next = nullptr;
    
    // Initialize physical and virtual memory managers
    PhysicalMemoryManager::initialize();
    VirtualMemoryManager::initialize();
}

void PhysicalMemoryManager::initialize() {
    // Initialize physical memory bitmap
    bitmap_size = (HEAP_SIZE / 4096) / 64 + 1;
    bitmap = (uint64_t*)heap;
    memset(bitmap, 0, bitmap_size * sizeof(uint64_t));
}

void* PhysicalMemoryManager::allocate_frame() {
    for (size_t i = 0; i < bitmap_size; i++) {
        if (bitmap[i] != ~0ULL) {
            for (int j = 0; j < 64; j++) {
                if (!(bitmap[i] & (1ULL << j))) {
                    bitmap[i] |= (1ULL << j);
                    return (void*)(((i * 64) + j) * 4096);
                }
            }
        }
    }
    return nullptr;
}

void PhysicalMemoryManager::free_frame(void* frame) {
    uint64_t frame_addr = (uint64_t)frame;
    uint64_t frame_index = frame_addr / 4096;
    bitmap[frame_index / 64] &= ~(1ULL << (frame_index % 64));
}

void VirtualMemoryManager::initialize() {
    create_page_tables();
}

void VirtualMemoryManager::map_page(uint64_t virtual_addr, uint64_t physical_addr) {
    uint64_t* pml4 = get_pml4();
    uint64_t pml4_index = (virtual_addr >> 39) & 0x1FF;
    uint64_t pdpt_index = (virtual_addr >> 30) & 0x1FF;
    uint64_t pd_index = (virtual_addr >> 21) & 0x1FF;
    uint64_t pt_index = (virtual_addr >> 12) & 0x1FF;

    // Implement page table manipulation
    if (!(pml4[pml4_index] & 0x1)) {
        // Allocate a new page for PDPT
        uint64_t* new_pdpt = (uint64_t*)PhysicalMemoryManager::allocate_frame();
        pml4[pml4_index] = (uint64_t)new_pdpt | 0x3; // Present + Writable
        memset(new_pdpt, 0, 4096);
    }
    uint64_t* pdpt = (uint64_t*)(pml4[pml4_index] & ~0xFFF);
    if (!(pdpt[pdpt_index] & 0x1)) {
        // Allocate a new page for PD
        uint64_t* new_pd = (uint64_t*)PhysicalMemoryManager::allocate_frame();
        pdpt[pdpt_index] = (uint64_t)new_pd | 0x3; // Present + Writable
        memset(new_pd, 0, 4096);
    }
    uint64_t* pd = (uint64_t*)(pdpt[pdpt_index] & ~0xFFF);
    if (!(pd[pd_index] & 0x1)) {
        // Allocate a new page for PT
        uint64_t* new_pt = (uint64_t*)PhysicalMemoryManager::allocate_frame();
        pd[pd_index] = (uint64_t)new_pt | 0x3; // Present + Writable
        memset(new_pt, 0, 4096);
    }
    uint64_t* pt = (uint64_t*)(pd[pd_index] & ~0xFFF);
    pt[pt_index] = physical_addr | 0x3; // Present + Writable
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
