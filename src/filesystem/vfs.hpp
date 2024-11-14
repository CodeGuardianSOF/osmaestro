#ifndef VFS_H
#define VFS_H

#include <stdint.h>
#include <stddef.h>

// File system structures
struct file_node {
    char name[256];
    uint32_t size;
    uint32_t flags;
    uint32_t read;
    uint32_t write;
    struct file_node* ptr;
};

// VFS functions
void vfs_init();
file_node* vfs_open(const char* path);
void vfs_close(file_node* node);
size_t vfs_read(file_node* node, uint8_t* buffer, size_t size);
size_t vfs_write(file_node* node, uint8_t* buffer, size_t size);

#endif