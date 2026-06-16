; boot.asm
section .multiboot_header
align 8

    dd 0xE85250D6                ; Multiboot2 magic
    dd 0                         ; architecture (i386)
    dd header_end - header_start ; total header length
    dd -(0xE85250D6 + 0 + (header_end - header_start))

header_start:

    ; Required end tag
    dw 0
    dw 0
    dd 8

header_end:

section .bss
    align 16
stack_bottom:
    resb 16384           ; 16KB stack
stack_top:

section .text
extern kernel_main
global _start
_start:
    mov esp, stack_top
    call kernel_main

.hang:
    cli
    hlt
    jmp .hang