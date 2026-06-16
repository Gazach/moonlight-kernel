

//kernel header
#include "terminal.h"

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

void kernel_main(void) 
{
	terminal_initialize();

	/* Print 35 lines — 10 past the 25-line VGA limit — to force scrolling */
	for (int i = 1; i <= 35; i++) {
		printf("Line %d: scrolling test - Moonlight OS\n", i);
	}

	printf(">>> scroll done. last visible line should be line 35.\n");
}