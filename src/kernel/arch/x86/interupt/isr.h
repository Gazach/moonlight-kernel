#ifndef ISR_H
#define ISR_H

#include <stdint.h>

// matches what isr_common pushes onto the stack
typedef struct {
    // pushed by isr_common (in reverse order)
    uint32_t gs, fs, es, ds;
    uint32_t edi, esi, ebp, esp;
    uint32_t ebx, edx, ecx, eax;
    // pushed by ISR stub
    uint32_t int_no;
    uint32_t err_code;
    // pushed automatically by CPU
    uint32_t eip, cs, eflags;
} __attribute__((packed)) interrupt_frame_t;

void isr_handler(interrupt_frame_t* frame);
void dump_registers(interrupt_frame_t* frame);

#endif