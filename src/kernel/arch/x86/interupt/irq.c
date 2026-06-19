#include "irq.h"
#include "pic.h"
#include "../../../terminal.h"

typedef void (*irq_handler_t)(void);
static irq_handler_t irq_handlers[16] = {0};

void irq_install(uint8_t irq, irq_handler_t handler) {
    irq_handlers[irq] = handler;
}

void irq_handler(uint8_t irq) {
    if (irq_handlers[irq])
        irq_handlers[irq]();
    pic_send_eoi(irq);
}

void irq_init(void) {}