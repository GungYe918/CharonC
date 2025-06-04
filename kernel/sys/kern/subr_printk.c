#include "../include/subr_printk.h"
#include <stdarg.h>
#include <stdio.h>      // vsnprintf 사용을 위함
#include <serial.h>     // serial 출력을 통한 디버그

static int print_uint_hex(unsigned long num, int width) {
    char hex[] = "0123456789abcdef";
    char buf[32];
    int i = 0;
    vt_t *vt = get_vt();

    if (num == 0) {
        vt_putc(vt, '0');
        serial_write_char('0');
        return -1;
    }

    while (num > 0 && i < (int)sizeof(buf)) {
        buf[i++] = hex[num % 16];
        num /= 16;
    }

    while (i < width) buf[i++] = '0'; // zero-padding

    while (--i >= 0) {
        vt_putc(vt, buf[i]);
        serial_write_char(buf[i]);
    }

    return 0;
}

static int print_int(int num) {
    char buf[16];
    int i = 0;
    unsigned int n;
    vt_t *vt = get_vt();

    if (num < 0) {
        vt_putc(vt, '-');
        serial_write_char('-');
        n = -num;
    } else {
        n = num;
    }

    if (n == 0) {
        vt_putc(vt, '0');
        serial_write_char('0');
        return -1;
    }

    while (n > 0 && i < (int)sizeof(buf)) {
        buf[i++] = '0' + (n % 10);
        n /= 10;
    }

    while (--i >= 0) {
        vt_putc(vt, buf[i]);
        serial_write_char(buf[i]);
    }

    return 0;
}

int printk(const char *fmt, ...) {
    vt_t *vt = get_vt();
    if (!vt) return -1;


    va_list ap;
    va_start(ap, fmt);

    for (; *fmt; ++fmt) {
        if (*fmt != '%') {
            vt_putc(vt, *fmt);
            serial_write_char(*fmt);
            continue;
        }

        ++fmt;

        switch (*fmt) {
            case 'd' :
                print_int(va_arg(ap, int));
                break;
            case 'x' :
                print_uint_hex(va_arg(ap, unsigned int), 0);
                break;
            case 'l' :
                if (*(fmt + 1) == 'x') {
                    ++fmt;
                    print_uint_hex(va_arg(ap, unsigned long), 0);
                }
                break;
            case 's' :
                const char* s = va_arg(ap, const char*);
                vt_puts(vt, s ? s : "(null)");
                break;
            case 'c' :
                vt_putc(vt, (char)va_arg(ap, int));
                break;
            case '%' :
                vt_putc(vt, '%');
                serial_write_char('%');
                break;
            default :
                vt_putc(vt, '?');
                serial_write_char('?');
                break;
        }
    }

    va_end(ap);
    return 1;
}

