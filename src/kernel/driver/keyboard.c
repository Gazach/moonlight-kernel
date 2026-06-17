#include "keyboard.h"
#include "ps2.h"
#include "../arch/io.h"
#include "../arch/interupt/idt.h"
#include "../arch/interupt/irq.h"
#include "../arch/interupt/pic.h"
#include "../terminal.h"


/// Set 2 scancodes
#define KEY_LSHIFT    0x12
#define KEY_RSHIFT    0x59
#define KEY_CAPS      0x58
#define KEY_ENTER     0x5A
#define KEY_BACKSPACE 0x66
#define KEY_ESC       0x76
#define KEY_TAB       0x0D
#define KEY_SPACE     0x29
#define KEY_LCTRL     0x14
#define KEY_LALT      0x11
#define KEY_RELEASE   0xF0

static unsigned char unshifted[67][2] = {
    // Numbers row
    {0x16,'1'},{0x1e,'2'},{0x26,'3'},{0x25,'4'},{0x2e,'5'},{0x36,'6'},{0x3d,'7'},{0x3e,'8'},{0x46,'9'},{0x45,'0'},
    // Top letter row (QWERTY...)
    {0x15,'q'},{0x1d,'w'},{0x24,'e'},{0x2d,'r'},{0x2c,'t'},{0x35,'y'},{0x3c,'u'},{0x43,'i'},{0x44,'o'},{0x4d,'p'},
    // Second letter row (ASDFGH...)
    {0x1c,'a'},{0x1b,'s'},{0x23,'d'},{0x2b,'f'},{0x34,'g'},{0x33,'h'},{0x3b,'j'},{0x42,'k'},{0x4b,'l'},
    // Third letter row (ZXCVBN...)
    {0x1a,'z'},{0x22,'x'},{0x21,'c'},{0x2a,'v'},{0x32,'b'},{0x31,'n'},{0x3a,'m'},
    // Special keys
    {0x29,' '},{0x0d,'\t'},{0x0e,'`'},{0x4e,'-'},{0x55,'='},{0x54,'['},{0x5b,']'},{0x5d,'\\'},
    {0x4c,';'},{0x52,'\''},{0x41,','},{0x49,'.'},{0x4a,'/'},
    {0x66,'\b'},{0x5a,'\n'},
    {0,0}
};

static unsigned char shifted[67][2] = {
    // Numbers row with shift (symbols)
    {0x16,'!'},{0x1e,'@'},{0x26,'#'},{0x25,'$'},{0x2e,'%'},{0x36,'^'},{0x3d,'&'},{0x3e,'*'},{0x46,'('},{0x45,')'},
    // Top letter row (QWERTY...)
    {0x15,'Q'},{0x1d,'W'},{0x24,'E'},{0x2d,'R'},{0x2c,'T'},{0x35,'Y'},{0x3c,'U'},{0x43,'I'},{0x44,'O'},{0x4d,'P'},
    // Second letter row (ASDFGH...)
    {0x1c,'A'},{0x1b,'S'},{0x23,'D'},{0x2b,'F'},{0x34,'G'},{0x33,'H'},{0x3b,'J'},{0x42,'K'},{0x4b,'L'},
    // Third letter row (ZXCVBN...)
    {0x1a,'Z'},{0x22,'X'},{0x21,'C'},{0x2a,'V'},{0x32,'B'},{0x31,'N'},{0x3a,'M'},
    // Special keys with shift
    {0x29,' '},{0x0d,'\t'},{0x0e,'~'},{0x4e,'_'},{0x55,'+'},{0x54,'{'},{0x5b,'}'},{0x5d,'|'},
    {0x4c,':'},{0x52,'"'},{0x41,'<'},{0x49,'>'},{0x4a,'?'},
    {0x66,'\b'},{0x5a,'\n'},
    {0,0}
};

static int shift_pressed = 0;
static int caps_lock     = 0;
static int release_next  = 0;

static char lookup(uint8_t scancode, int use_shift) {
    unsigned char (*table)[2] = use_shift ? shifted : unshifted;
    for (int i = 0; table[i][0] != 0; i++)
        if (table[i][0] == scancode)
            return (char)table[i][1];
    return 0;
}

static void keyboard_handler(void) {
    uint8_t scancode = PS2_ReadData();

    // 0xF0 = release prefix, next byte is the key that was released
    if (scancode == KEY_RELEASE) {
        release_next = 1;
        return;
    }

    if (release_next) {
        release_next = 0;
        if (scancode == KEY_LSHIFT || scancode == KEY_RSHIFT)
            shift_pressed = 0;
        return;
    }

    // key press
    switch (scancode) {
        case KEY_LSHIFT:
        case KEY_RSHIFT:
            shift_pressed = 1;
            return;
        case KEY_CAPS:
            caps_lock = !caps_lock;
            return;
        case KEY_ESC:
        case KEY_LCTRL:
        case KEY_LALT:
            return;  // ignore for now
    }

    char c = lookup(scancode, shift_pressed ^ caps_lock);
    if (c) terminal_putchar(c);
}

void keyboard_init(void) {
    irq_install(1, keyboard_handler);
    pic_unmask(1);
}