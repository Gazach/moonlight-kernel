#ifndef IRQ_H
#define IRQ_H

#include <stdint.h>

void irq_init(void);
void irq_handler(uint32_t irq);

#endif