#include "kbd.h"
#include "kbd_map.h"

#define PS2_DATA_PORT 0x60
#define PS2_STATUS_PORT 0x64



// 키보드 초기화
int kbd_init() {
    printk("[DEVICE] Initializing PS/2 Keyboard...\n");

    // 첫 번째 PS/2 port 활성화
    outb(KBD_CMD_PORT, KBD_ENABLE_CMD);

    // input buffer 초기화때까지 대기
    while (inb(KBD_STATUS_PORT) & 0x02);

    // 키보드 인터럽트 활성화
    outb(KBD_DATA_PORT, 0xF4);

    printk("[KBD] PS/2 Keyboard Initialized.\n");

    return 0;
}

#define CAPSLOCK_SCANCODE 0x3A
#define LSHIFT_PRESS  0x2A
#define RSHIFT_PRESS  0x36
#define LSHIFT_RELEASE 0xAA
#define RSHIFT_RELEASE 0xB6

extern const char scancode_table[128];
extern const char scancode_table_shift[128];

static int shift_pressed = 0;
static int capslock_on = 0;

int kbd_handler() {
    uint8_t scancode = inb(PS2_DATA_PORT);

    // Shift 키 처리
    if (scancode == LSHIFT_PRESS || scancode == RSHIFT_PRESS) {
        shift_pressed = 1;
    } else if (scancode == LSHIFT_RELEASE || scancode == RSHIFT_RELEASE) {
        shift_pressed = 0;
    }
    // Caps Lock 토글
    else if (scancode == CAPSLOCK_SCANCODE) {
        capslock_on = !capslock_on;
    }
    // 일반 키 처리
    else if (!(scancode & 0x80)) {
        char c;

        if (shift_pressed) {
            c = scancode_table_shift[scancode];
        } else {
            c = scancode_table[scancode];
        }

        // 대소문자 조정 (Shift XOR CapsLock)
        if (c >= 'a' && c <= 'z') {
            if (shift_pressed ^ capslock_on) {
                c = c - 'a' + 'A';
            }
        }

        if (c) {
            printk("%c", c);
        } else {
            printk("[?]");
        }
    }

    outb(0x20, 0x20); // PIC EOI
    return 0;
}