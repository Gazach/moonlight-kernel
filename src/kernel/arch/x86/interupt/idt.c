#include "IDT.h"
#include "isr.h"
#include <stdint.h>

static struct idt_entry idt[IDT_SIZE];
static struct idt_ptr idt_ptr;

extern void isr_stub(void); 

// ISR stubs for CPU exceptions (0-31)
extern void isr_stub_0(void);
extern void isr_stub_1(void);
extern void isr_stub_2(void);
extern void isr_stub_3(void);
extern void isr_stub_4(void);
extern void isr_stub_5(void);
extern void isr_stub_6(void);
extern void isr_stub_7(void);
extern void isr_stub_8(void);
extern void isr_stub_9(void);
extern void isr_stub_10(void);
extern void isr_stub_11(void);
extern void isr_stub_12(void);
extern void isr_stub_13(void);
extern void isr_stub_14(void);
extern void isr_stub_15(void);
extern void isr_stub_16(void);
extern void isr_stub_17(void);
extern void isr_stub_18(void);
extern void isr_stub_19(void);
extern void isr_stub_20(void);
extern void isr_stub_21(void);
extern void isr_stub_22(void);
extern void isr_stub_23(void);
extern void isr_stub_24(void);
extern void isr_stub_25(void);
extern void isr_stub_26(void);
extern void isr_stub_27(void);
extern void isr_stub_28(void);
extern void isr_stub_29(void);
extern void isr_stub_30(void);
extern void isr_stub_31(void);

extern void irq_stub_0(void);
extern void irq_stub_1(void);
extern void irq_stub_2(void);
extern void irq_stub_3(void);
extern void irq_stub_4(void);
extern void irq_stub_5(void);
extern void irq_stub_6(void);
extern void irq_stub_7(void);
extern void irq_stub_8(void);
extern void irq_stub_9(void);
extern void irq_stub_10(void);
extern void irq_stub_11(void);
extern void irq_stub_12(void);
extern void irq_stub_13(void);
extern void irq_stub_14(void);
extern void irq_stub_15(void);



// helper function to set an entry in the IDT
void set_idt_entry(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].offset_low = base & 0xFFFF;
    idt[num].selector = sel;
    idt[num].zero = 0;
    idt[num].type_attr = flags;
    idt[num].offset_high = (base >> 16) & 0xFFFF;
}

// Initialize the IDT with a simple handler for all interrupts
void idt_init() {
    idt_ptr.limit = sizeof(struct idt_entry) * IDT_SIZE - 1;
    idt_ptr.base  = (uint32_t)(uintptr_t)&idt;

    // default handler for everything
    for (int i = 0; i < IDT_SIZE; i++)
        set_idt_entry(i, (uint32_t)(uintptr_t)isr_stub_0, 0x10, 0x8E);

    // CPU exceptions 0-31
    set_idt_entry(0,  (uint32_t)(uintptr_t)isr_stub_0,  0x10, 0x8E);
    set_idt_entry(1,  (uint32_t)(uintptr_t)isr_stub_1,  0x10, 0x8E);
    set_idt_entry(2,  (uint32_t)(uintptr_t)isr_stub_2,  0x10, 0x8E);
    set_idt_entry(3,  (uint32_t)(uintptr_t)isr_stub_3,  0x10, 0x8E);
    set_idt_entry(4,  (uint32_t)(uintptr_t)isr_stub_4,  0x10, 0x8E);
    set_idt_entry(5,  (uint32_t)(uintptr_t)isr_stub_5,  0x10, 0x8E);
    set_idt_entry(6,  (uint32_t)(uintptr_t)isr_stub_6,  0x10, 0x8E);
    set_idt_entry(7,  (uint32_t)(uintptr_t)isr_stub_7,  0x10, 0x8E);
    set_idt_entry(8,  (uint32_t)(uintptr_t)isr_stub_8,  0x10, 0x8E);
    set_idt_entry(9,  (uint32_t)(uintptr_t)isr_stub_9,  0x10, 0x8E);
    set_idt_entry(10, (uint32_t)(uintptr_t)isr_stub_10, 0x10, 0x8E);
    set_idt_entry(11, (uint32_t)(uintptr_t)isr_stub_11, 0x10, 0x8E);
    set_idt_entry(12, (uint32_t)(uintptr_t)isr_stub_12, 0x10, 0x8E);
    set_idt_entry(13, (uint32_t)(uintptr_t)isr_stub_13, 0x10, 0x8E);
    set_idt_entry(14, (uint32_t)(uintptr_t)isr_stub_14, 0x10, 0x8E);
    set_idt_entry(15, (uint32_t)(uintptr_t)isr_stub_15, 0x10, 0x8E);
    set_idt_entry(16, (uint32_t)(uintptr_t)isr_stub_16, 0x10, 0x8E);
    set_idt_entry(17, (uint32_t)(uintptr_t)isr_stub_17, 0x10, 0x8E);
    set_idt_entry(18, (uint32_t)(uintptr_t)isr_stub_18, 0x10, 0x8E);
    set_idt_entry(19, (uint32_t)(uintptr_t)isr_stub_19, 0x10, 0x8E);
    set_idt_entry(20, (uint32_t)(uintptr_t)isr_stub_20, 0x10, 0x8E);
    set_idt_entry(21, (uint32_t)(uintptr_t)isr_stub_21, 0x10, 0x8E);
    set_idt_entry(22, (uint32_t)(uintptr_t)isr_stub_22, 0x10, 0x8E);
    set_idt_entry(23, (uint32_t)(uintptr_t)isr_stub_23, 0x10, 0x8E);
    set_idt_entry(24, (uint32_t)(uintptr_t)isr_stub_24, 0x10, 0x8E);
    set_idt_entry(25, (uint32_t)(uintptr_t)isr_stub_25, 0x10, 0x8E);
    set_idt_entry(26, (uint32_t)(uintptr_t)isr_stub_26, 0x10, 0x8E);
    set_idt_entry(27, (uint32_t)(uintptr_t)isr_stub_27, 0x10, 0x8E);
    set_idt_entry(28, (uint32_t)(uintptr_t)isr_stub_28, 0x10, 0x8E);
    set_idt_entry(29, (uint32_t)(uintptr_t)isr_stub_29, 0x10, 0x8E);
    set_idt_entry(30, (uint32_t)(uintptr_t)isr_stub_30, 0x10, 0x8E);
    set_idt_entry(31, (uint32_t)(uintptr_t)isr_stub_31, 0x10, 0x8E);

    // IRQs 0-15 at vectors 32-47
    set_idt_entry(32, (uint32_t)(uintptr_t)irq_stub_0,  0x10, 0x8E);
    set_idt_entry(33, (uint32_t)(uintptr_t)irq_stub_1,  0x10, 0x8E);
    set_idt_entry(34, (uint32_t)(uintptr_t)irq_stub_2,  0x10, 0x8E);
    set_idt_entry(35, (uint32_t)(uintptr_t)irq_stub_3,  0x10, 0x8E);
    set_idt_entry(36, (uint32_t)(uintptr_t)irq_stub_4,  0x10, 0x8E);
    set_idt_entry(37, (uint32_t)(uintptr_t)irq_stub_5,  0x10, 0x8E);
    set_idt_entry(38, (uint32_t)(uintptr_t)irq_stub_6,  0x10, 0x8E);
    set_idt_entry(39, (uint32_t)(uintptr_t)irq_stub_7,  0x10, 0x8E);
    set_idt_entry(40, (uint32_t)(uintptr_t)irq_stub_8,  0x10, 0x8E);
    set_idt_entry(41, (uint32_t)(uintptr_t)irq_stub_9,  0x10, 0x8E);
    set_idt_entry(42, (uint32_t)(uintptr_t)irq_stub_10, 0x10, 0x8E);
    set_idt_entry(43, (uint32_t)(uintptr_t)irq_stub_11, 0x10, 0x8E);
    set_idt_entry(44, (uint32_t)(uintptr_t)irq_stub_12, 0x10, 0x8E);
    set_idt_entry(45, (uint32_t)(uintptr_t)irq_stub_13, 0x10, 0x8E);
    set_idt_entry(46, (uint32_t)(uintptr_t)irq_stub_14, 0x10, 0x8E);
    set_idt_entry(47, (uint32_t)(uintptr_t)irq_stub_15, 0x10, 0x8E);

    __asm__ volatile("lidtl (%0)" : : "r"(&idt_ptr));
}