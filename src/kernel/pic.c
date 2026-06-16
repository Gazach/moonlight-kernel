#include "pic.h"

// Write to I/O port
static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

// Read from I/O port
static inline uint8_t inb(uint16_t port) {
    uint8_t val;
    __asm__ volatile("inb %1, %0" : "=a"(val) : "Nd"(port));
    return val;
}

// Small delay for old hardware
static inline void io_wait(void) {
    outb(0x80, 0);
}

void pic_init(void) {
    // ICW1 — start initialization sequence
    outb(PIC1_COMMAND, 0x11); io_wait();
    outb(PIC2_COMMAND, 0x11); io_wait();

    // ICW2 — remap IRQs
    // Master PIC: IRQ0-7  → vectors 32-39
    // Slave PIC:  IRQ8-15 → vectors 40-47
    outb(PIC1_DATA, 32); io_wait();
    outb(PIC2_DATA, 40); io_wait();

    // ICW3 — tell PICs about each other
    outb(PIC1_DATA, 0x04); io_wait(); // master: slave on IRQ2
    outb(PIC2_DATA, 0x02); io_wait(); // slave: cascade identity

    // ICW4 — 8086 mode
    outb(PIC1_DATA, 0x01); io_wait();
    outb(PIC2_DATA, 0x01); io_wait();

    // Mask all IRQs for now (we'll unmask as needed)
    outb(PIC1_DATA, 0xFF);
    outb(PIC2_DATA, 0xFF);
}

void pic_send_eoi(uint8_t irq) {
    if (irq >= 8)
        outb(PIC2_COMMAND, PIC_EOI); // tell slave too
    outb(PIC1_COMMAND, PIC_EOI);
}

void pic_mask(uint8_t irq) {
    uint16_t port = (irq < 8) ? PIC1_DATA : PIC2_DATA;
    if (irq >= 8) irq -= 8;
    outb(port, inb(port) | (1 << irq));
}

void pic_unmask(uint8_t irq) {
    uint16_t port = (irq < 8) ? PIC1_DATA : PIC2_DATA;
    if (irq >= 8) irq -= 8;
    outb(port, inb(port) & ~(1 << irq));
}