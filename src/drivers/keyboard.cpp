#include "keyboard.hpp"
#include "../io/ports.hpp"
#include "../system/interrupt.hpp"

static uint8_t keyboard_buffer[256];
static uint8_t buffer_start = 0;
static uint8_t buffer_end = 0;

// Scancode to ASCII mapping
static const char scancode_ascii[] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    '*', 0, ' '
};

void keyboard_initialize() {
    // Register keyboard interrupt handler
    register_interrupt_handler(33, keyboard_handler);
    
    // Enable keyboard
    while(inb(KEYBOARD_STATUS_PORT) & 2);
    outb(KEYBOARD_COMMAND_PORT, KEYBOARD_CMD_ENABLE);
    
    // Wait for acknowledgment
    while(inb(KEYBOARD_DATA_PORT) != 0xFA);
}

void keyboard_handler() {
    uint8_t scancode = inb(KEYBOARD_DATA_PORT);
    
    if(scancode < sizeof(scancode_ascii)) {
        keyboard_buffer[buffer_end++] = scancode_ascii[scancode];
        if(buffer_end >= sizeof(keyboard_buffer)) buffer_end = 0;
    }
}

char keyboard_get_char() {
    if(buffer_start == buffer_end) return 0;
    
    char c = keyboard_buffer[buffer_start++];
    if(buffer_start >= sizeof(keyboard_buffer)) buffer_start = 0;
    return c;
}

void keyboard_check() {
    char c;
    while((c = keyboard_get_char()) != 0) {
        // Process keyboard input
        // Add your keyboard input handling code here
    }
}