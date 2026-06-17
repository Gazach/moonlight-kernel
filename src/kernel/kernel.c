

//kernel header
#include "terminal.h"
#include "arch/interupt/idt.h"
#include "arch/interupt/pic.h"
#include "arch/interupt/irq.h"

//driver header
#include "driver/ps2.h"
#include "driver/keyboard.h"

// libary headers
#include "std/printf.h"
#include "common/string.h"

// Keyboard driver test
void keyboard_test(void) {
    printf("\n========================================\n");
    printf("  Keyboard Driver Test\n");
    printf("========================================\n");
    printf("\nInstructions:\n");
    printf("1. Type some characters (should echo)\n");
    printf("2. Try SHIFT for uppercase\n");
    printf("3. Try CAPS LOCK\n");
    printf("4. Use BACKSPACE to delete\n");
    printf("5. Press ENTER when done\n");
    printf("\nStart typing: ");
    
    int enter_pressed = 0;
    int char_count = 0;
    
    // Simple input loop - continue until user presses ENTER
    // The keyboard interrupt handler will automatically call terminal_putchar()
    // So we just need to detect ENTER
    // For now, just wait a bit and exit
    printf("\n\n(Test: Characters should appear as you type)\n");
}



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
    PS2_init();
    keyboard_init();
    
    __asm__ volatile("sti");

    printf("Moonlight OS!\n");
    printf("Keyboard driver initialized.\n");
    
    // Run keyboard test
    keyboard_test();
    
    printf("\nKernel ready. Type something:\n> ");
    
    for(;;);
}