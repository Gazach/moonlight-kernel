#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

// keyboard public API
void keyboard_init();
char keyboard_getchar();
int keyboard_readline(char* buf, int max);

#endif // KEYBOARD_H