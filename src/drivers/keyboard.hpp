#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

// Keyboard ports
#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
#define KEYBOARD_COMMAND_PORT 0x64

// Keyboard commands
#define KEYBOARD_CMD_LED 0xED
#define KEYBOARD_CMD_ECHO 0xEE
#define KEYBOARD_CMD_SCANCODE 0xF0
#define KEYBOARD_CMD_IDENTIFY 0xF2
#define KEYBOARD_CMD_TYPEMATIC 0xF3
#define KEYBOARD_CMD_ENABLE 0xF4
#define KEYBOARD_CMD_DISABLE 0xF5
#define KEYBOARD_CMD_DEFAULT 0xF6
#define KEYBOARD_CMD_RESEND 0xFE
#define KEYBOARD_CMD_RESET 0xFF

// Function declarations
void keyboard_initialize();
void keyboard_handler();
void keyboard_check();
char keyboard_get_char();
void keyboard_set_leds(uint8_t status);

#endif