#include "../include/serial.h"
#include <io/io.h>

#define COM1_PORT 0x3F8


void serial_init(void) {
    outb(COM1_PORT + 1, 0x00); // disable interrupts
    outb(COM1_PORT + 3, 0x80); // enable DLAB
    outb(COM1_PORT + 0, 0x03); // divisor low byte (38400)
    outb(COM1_PORT + 1, 0x00); // divisor high byte
    outb(COM1_PORT + 3, 0x03); // 8N1
    outb(COM1_PORT + 2, 0xC7); // FIFO enable
    outb(COM1_PORT + 4, 0x0B); // IRQs enabled, RTS/DSR set
}

int serial_is_transmit_ready() {
    return inb(COM1_PORT + 5) & 0x20;
}

void serial_write_char(char c) {
    while (!serial_is_transmit_ready());
    outb(COM1_PORT, c);
}

void serial_write(const char* str) {
    while (*str) {
        if (*str == '\n') serial_write_char('\r');
        serial_write_char(*str++);
    }
}

void serial_write_hex(uint64_t val) {
    const char* hex = "0123456789ABCDEF";
    char buf[17];
    buf[16] = '\0';
    for (int i = 15; i >= 0; i--) {
        buf[i] = hex[val & 0xF];
        val >>= 4;
    }
    serial_write("0x");
    serial_write(buf);
}