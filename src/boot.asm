; Multiboot header for GRUB
section .multiboot
align 8
dd 0x1BADB002              ; Magic number
dd 0x00                    ; Flags
dd -(0x1BADB002 + 0x00)   ; Checksum

section .bss
align 16
stack_bottom:
    resb 32768            ; 32 KB stack
stack_top:

section .text
global _start
extern kernel_main

; GDT for 64-bit mode
GDT64:
    dq 0                  ; Null descriptor
.Code:
    dq (1 << 43) | (1 << 44) | (1 << 47) | (1 << 53) ; Code segment

GDT64Pointer:
    dw $ - GDT64 - 1     ; GDT length
    dq GDT64             ; GDT address

; Page tables
align 4096
PML4:
    dq PDPT + 0x3        ; Present + Writable
    times 511 dq 0       ; Rest of entries are empty

PDPT:
    dq PD + 0x3          ; Present + Writable
    times 511 dq 0

PD:
    dq PT + 0x3          ; Present + Writable
    times 511 dq 0

PT:
    %assign i 0
    %rep 512
        dq (i << 12) | 0x3
        %assign i i+1
    %endrep

_start:
    ; Set up stack
    mov esp, stack_top

    ; Check for multiboot
    cmp eax, 0x2BADB002
    jne .no_multiboot
    push ebx              ; Save multiboot info pointer

    ; Enable PAE
    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    ; Set long mode bit
    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    ; Load PML4
    mov eax, PML4
    mov cr3, eax

    ; Enable paging
    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax

    ; Load GDT
    lgdt [GDT64Pointer]

    ; Long jump to 64-bit code
    jmp 0x08:.long_mode

.no_multiboot:
    ; Handle error
    hlt

bits 64
.long_mode:
    ; Update segment registers
    mov ax, 0x10
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Call kernel
    call kernel_main
    
    ; Halt if kernel returns
    cli
    hlt

.hang:
    jmp .hang