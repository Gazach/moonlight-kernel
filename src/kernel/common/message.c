#include "../terminal.h"
#include "../std/printf.h"


// PUBLIC API

// Hello message to greet users like neofetch

void print_welcome_message()
{
    int radius_y = 5;
    int radius_x = 8;

    const char* info[] = {
        "Moonlight OS",
        "-------------",
        "Version : 0.1",
        "Arch    : i386",
        "Shell   : MoonShell",
        "",
        "Type 'help' to find more about command system",
    };

    int info_count = sizeof(info) / sizeof(info[0]);
    int line = 0;

    for (int y = -radius_y; y <= radius_y; y++)
    {
        for (int x = -radius_x; x <= radius_x; x++)
        {
            int dist_sq = x * x + y * y;
            int radius_sq = radius_y * radius_y;
            
            // minecraft ahh circle algorithm
            if (dist_sq < radius_sq)
                terminal_putchar('*');
            else
                terminal_putchar(' ');
        }

        // print info lines next to the circle

        terminal_putchar(' ');
        terminal_putchar(' ');

        if (line < info_count)
            printf("%s", info[line]);

        terminal_putchar('\n');
        line++;
    }

    terminal_putchar('\n');
}

// show command list

static void help_list_message(void) {
    printf("Commands:\n");
    printf("  help       show this message\n");
    printf("  clear      clear the screen\n");
    printf("  echo       print arguments\n");
    printf("  about      show OS info\n");
}