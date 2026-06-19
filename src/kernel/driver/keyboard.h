#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

// keyboard public API
void keyboard_init();
char keyboard_getchar();
int keyboard_readline(char* buf, int max);

// IRQ registration — called by irq_init
void keyboard_irq_init(void);

#endif // KEYBOARD_H