extern isr_handler
extern irq_handler

section .text

; macro for exceptions WITHOUT error code — push 0 as dummy
%macro ISR_NOERR 1
global isr_stub_%1
isr_stub_%1:
    push byte 0     ; dummy error code
    push byte %1    ; exception number
    jmp isr_common
%endmacro

; macro for exceptions WITH error code — CPU already pushed it
%macro ISR_ERR 1
global isr_stub_%1
isr_stub_%1:
    push byte %1    ; exception number
    jmp isr_common
%endmacro

; common handler — saves all registers and calls C handler
isr_common:
    pusha
    push ds
    push es
    push fs
    push gs
    push esp        ; pass pointer to frame as argument
    call isr_handler
    add esp, 4      ; clean up the push esp
    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8
    iret

; CPU exceptions 0-31
; exceptions that do NOT push error codes
ISR_NOERR  0   ; divide by zero
ISR_NOERR  1   ; debug
ISR_NOERR  2   ; NMI
ISR_NOERR  3   ; breakpoint
ISR_NOERR  4   ; overflow
ISR_NOERR  5   ; bound range exceeded
ISR_NOERR  6   ; invalid opcode
ISR_NOERR  7   ; device not available
ISR_ERR    8   ; double fault          <= pushes error code
ISR_NOERR  9   ; coprocessor overrun
ISR_ERR   10   ; invalid TSS           <= pushes error code
ISR_ERR   11   ; segment not present   <= pushes error code
ISR_ERR   12   ; stack fault           <= pushes error code
ISR_ERR   13   ; general protection    <= pushes error code
ISR_ERR   14   ; page fault            <= pushes error code
ISR_NOERR 15   ; reserved
ISR_NOERR 16   ; x87 floating point
ISR_ERR   17   ; alignment check       <= pushes error code
ISR_NOERR 18   ; machine check
ISR_NOERR 19   ; SIMD floating point
ISR_NOERR 20   ; virtualization
ISR_NOERR 21   ; reserved
ISR_NOERR 22   ; reserved
ISR_NOERR 23   ; reserved
ISR_NOERR 24   ; reserved
ISR_NOERR 25   ; reserved
ISR_NOERR 26   ; reserved
ISR_NOERR 27   ; reserved
ISR_NOERR 28   ; reserved
ISR_NOERR 29   ; reserved
ISR_ERR   30   ; security exception    <= pushes error code
ISR_NOERR 31   ; reserved

; IRQ stubs for hardware interrupts (IRQ0-IRQ15)
%macro IRQ 1
global irq_stub_%1
irq_stub_%1:
    push byte %1    ; push IRQ number (0-15)
    call irq_handler
    add esp, 4
    iret
%endmacro

IRQ  0
IRQ  1
IRQ  2
IRQ  3
IRQ  4
IRQ  5
IRQ  6
IRQ  7
IRQ  8
IRQ  9
IRQ 10
IRQ 11
IRQ 12
IRQ 13
IRQ 14
IRQ 15