#include <idt.h>
#include <isr.h>
#include <stdint.h>
#include <machdep.h>

int init_amd64(void) {
    idt_init();
    return 1;
}