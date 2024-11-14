; Multiboot header for GRUB
section .multiboot
align 4
dd 0x1BADB002              ; Magic number
dd 0x00                    ; Flags
dd -(0x1BADB002 + 0x00)   ; Checksum

section .text
global _start
extern kernel_main

_start:
    cli                    ; Disable interrupts
    mov esp, stack_top     ; Set up stack pointer
    push ebx              ; Push multiboot info pointer
    call kernel_main      ; Call our kernel
    hlt                   ; Halt the CPU

section .bss
align 16
stack_bottom:
    resb 16384           ; 16 KB stack
stack_top: