#include "IDT.h"
#include "pic.h"
#include "irq.h"
#include "../../terminal.h"

void isr_handler() {
    terminal_writestring("!!! interrupt caught !!!\n");
    // For now, just print a message. In a real OS, you'd want to handle the interrupt properly.
    __asm__ volatile("cli; hlt"); // Disable interrupts to prevent re-entrancy
}

void irq_handler(uint32_t irq) {
    terminal_writestring("!!! IRQ received !!!\n");
    pic_send_eoi(irq);
}

void irq_init(void) {
    /* IRQ stubs already registered in idt_init() at vectors 32-47 */
    /* PIC is initialized by pic_init() */
}