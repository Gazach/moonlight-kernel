#include "shell_cmd.h"

#include "../arch/x86/cpu.h"
#include "../memory/m_heap.h"
#include "../driver/timer.h"

// Basic shell commands

void cmd_help(void) {
    help_list_message();
}

void cmd_clear(void) {
    terminal_initialize();
}

void cmd_echo(int argc, char* argv[]) {
    for (int i = 1; i < argc; i++) {
        printf("%s", argv[i]);
        if (i < argc - 1) printf(" ");
    }
    printf("\n");
}

void cmd_about(void) {
    printf("Moonlight OS\n");
    printf("Arch: x86 (i686)\n");
    printf("Built with clang + nasm\n");
}

void cmd_version(void) {
    printf("Moonlight OS version: %d.%d.%d\n", KERNEL_VERSION_MAJOR, KERNEL_VERSION_MINOR, KERNEL_VERSION_PATCH);
}

void cmd_shell_version(void) {
    printf("MoonShell version: %d.%d.%d\n", SHELL_VERSION_MAJOR, SHELL_VERSION_MINOR, SHELL_VERSION_PATCH);
}

// Moonfetch command to display system information

void cmd_moonfetch(void) {
    moonfetch_message();

}

void cmd_uptime(void) {
    uint32_t ms   = timer_ticks();        // ticks = ms at 1000Hz
    uint32_t secs = ms / 1000;
    uint32_t mins = secs / 60;
    secs = secs % 60;
    printf("Uptime: %u:%02u.%03u\n", mins, secs, ms % 1000);
}

void cmd_sleep(int argc, char* argv[]) {
    if (argc < 2) {
        printf("usage: sleep <ms>\n");
        return;
    }
    uint32_t ms = atoi(argv[1]);
    printf("Sleeping %u ms...\n", ms);
    timer_sleep(ms);
    printf("Done!\n");
}