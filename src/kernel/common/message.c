#include "../terminal.h"
#include "../std/printf.h"
#include "../kernel_version.h"
#include "../arch/x86/cpu.h"
#include "../memory/pmm.h"
#include "../vga.h"

// PUBLIC API

// Hello message to greet users like neofetch

void print_welcome_message() {
    printf("Welcome to Moonlight OS! ^^\ntype ''help'' to find more commands.\n");
}

void moonfetch_message()
{
    int radius_y = 5;
    int radius_x = 8;

    char cpu_brand[CPU_BRAND_STRING_LENGTH];
    char cpu_vendor[CPU_VENDOR_STRING_LENGTH];

    cpu_get_brand(cpu_brand);
    cpu_get_vendor(cpu_vendor);

    printf("\n");
    
    for (int y = -radius_y; y <= radius_y; y++)
    {
        // Set moon to dark grey at the start of each row
        terminal_setcolor(vga_entry_color(VGA_COLOR_DARK_GREY, VGA_COLOR_BLACK));
        
        for (int x = -radius_x; x <= radius_x; x++)
        {
            int dist_sq = x * x + y * y;
            int radius_sq = radius_y * radius_y;
            
            if (dist_sq < radius_sq)
                terminal_putchar('*');
            else
                terminal_putchar(' ');
        }

        // Set text color to white
        terminal_setcolor(vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
        terminal_putchar(' ');
        terminal_putchar(' ');

        // Print system information next to the moon
        if (y == -radius_y + 0)
            printf("Moonlight OS");
        else if (y == -radius_y + 1)
            printf("------------------");
        else if (y == -radius_y + 2)
            printf("OS Version    : %d.%d.%d", KERNEL_VERSION_MAJOR, KERNEL_VERSION_MINOR, KERNEL_VERSION_PATCH);
        else if (y == -radius_y + 3)
            printf("Shell Name    : MoonShell");
        else if (y == -radius_y + 4)
            printf("Shell Version : %d.%d.%d", SHELL_VERSION_MAJOR, SHELL_VERSION_MINOR, SHELL_VERSION_PATCH);
        else if (y == -radius_y + 5)
            printf("Kernel Arch   : i686");
        else if (y == -radius_y + 6)
            printf("CPU Arch      : %s", cpu_is_x86_64() ? "x86_64" : "x86");
        else if (y == -radius_y + 7)
            printf("CPU Vendor    : %s", cpu_vendor);
        else if (y == -radius_y + 8)
            printf("CPU Brand     : %s", cpu_brand);
        else if (y == -radius_y + 9){
            uint32_t used_kb  = pmm_used_pages() * 4;
            uint32_t total_mb = (pmm_total_pages() * 4) / 1024;

                if (used_kb < 1024) {
                printf("Memory Usage: %u KB used / %u MB total", used_kb, total_mb);
            } else {
                printf("Memory Usage    : %u MB used / %u MB total", used_kb / 1024, total_mb);
            }
        }
        else if (y == -radius_y + 10){
            uint32_t free_kb = pmm_free_pages() * 4;
            if (free_kb < 1024)
                printf("Free Memory   : %u KB", free_kb);
            else
                printf("Free Memory   : %u MB", free_kb / 1024);
            terminal_putchar('\n');
        
        // Set color back to dark grey for next moon line
        terminal_setcolor(vga_entry_color(VGA_COLOR_DARK_GREY, VGA_COLOR_BLACK));
    }

    // Set text color back to white for next output
    terminal_setcolor(vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
    printf("\n");
}
}
// show command list

void help_list_message(void) {
    printf("Commands:\n");
    printf("  help       show this message\n");
    printf("  clear      clear the screen\n");
    printf("  echo       print arguments\n");
    printf("  about      show OS info\n");
    printf("  moonfetch  display system information\n");
    printf("  version    show OS version\n");
    printf("  shell-version   show shell version\n");
    printf("  uptime     show system uptime\n");
    printf("  sleep      sleep for specified milliseconds\n");
}