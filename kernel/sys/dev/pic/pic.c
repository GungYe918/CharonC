#include "pic.h"

void pic_remap() {
    // Master PIC command/data port: 0x20/0x21
    // Slave PIC command/data port: 0xA0/0xA1
    outb(0x20, 0x11);
    outb(0xA0, 0x11);

    outb(0x21, 0x20);  // Master offset = 0x20 (IRQ0-7 -> 0x20-0x27)
    outb(0xA1, 0x28);  // Slave offset  = 0x28 (IRQ8-15 -> 0x28-0x2F)

    outb(0x21, 0x04);  // Tell Master PIC that there is a slave PIC at IRQ2
    outb(0xA1, 0x02);  // Tell Slave PIC its cascade identity

    outb(0x21, 0x01);  // 8086/88 mode
    outb(0xA1, 0x01);  // 8086/88 mode

    // Unmask keyboard IRQ (IRQ1 = 2nd bit)
    outb(0x21, 0xFD);  // 1111 1101 (Only IRQ1 enabled)
    outb(0xA1, 0xFF);  // Mask all on slave PIC
}