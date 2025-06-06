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

static int shift_pressed = 0;

int kbd_handler() {
    uint8_t scancode = inb(PS2_DATA_PORT);

    // Shift 키 누름
    if (scancode == 0x2A || scancode == 0x36) {
        shift_pressed = 1;
        goto end;
    }

    // Shift 키 뗌
    if (scancode == 0xAA || scancode == 0xB6) {
        shift_pressed = 0;
        goto end;
    }

    // 릴리즈 코드 무시
    if (scancode & 0x80)
        goto end;

    char c = shift_pressed ? scancode_table_shift[scancode] : scancode_table[scancode];
    if (c) {
        printk("%c", c);
    } else {
        printk("[?]");
    }

end:
    outb(0x20, 0x20); // PIC EOI
    return 0;
}