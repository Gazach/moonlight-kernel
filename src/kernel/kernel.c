

//kernel header
#include "terminal.h"
#include "IDT.h"
#include "pic.h"
#include "irq.h"

#include "std/printf.h"
#include "common/string.h"


/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

void kernel_main(void) {
    terminal_initialize();
    idt_init();
    pic_init();
    irq_init();
    __asm__ volatile("sti");

    printf("IRQs ready\n");
    for(;;);
}