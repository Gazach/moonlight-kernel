// PIT (Programmable Interval Timer) driver for x86 architecture

#include "timer.h"
#include "../arch/x86/interupt/irq.h"
#include "../arch/x86/interupt/pic.h"
#include "../arch/x86/io.h"

// PIT ports
#define PIT_CHANNEL0  0x40  // IRQ0
#define PIT_COMMAND   0x43

// PIT base frequency
#define PIT_BASE_HZ   1193182

static uint32_t tick_count = 0;
static uint32_t tick_hz    = 0;

static void timer_handler(void) {
    tick_count++;
}

// hardware setup only
void timer_init(uint32_t hz) {
    tick_hz = hz;
    uint32_t divisor = PIT_BASE_HZ / hz;
    outb(PIT_COMMAND, 0x36);
    outb(PIT_CHANNEL0, (uint8_t)(divisor & 0xFF));
    outb(PIT_CHANNEL0, (uint8_t)((divisor >> 8) & 0xFF));
}

// IRQ registration — called by irq_init
void timer_irq_init(void) {
    irq_install(0, timer_handler);
    pic_unmask(0);
}

// return the number of ticks since boot
uint32_t timer_ticks(void) {
    return tick_count;
}

// sleep for a specified number of milliseconds
void timer_sleep(uint32_t ms) {
    uint32_t ticks_to_wait = (tick_hz * ms) / 1000;
    uint32_t start = tick_count;
    while ((tick_count - start) < ticks_to_wait) {
        __asm__ volatile("hlt");  // wait for next interrupt
    }
}