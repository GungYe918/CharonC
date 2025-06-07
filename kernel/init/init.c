#include "Lib/boot/BootInfo.h"
#include "sys/dev/fb/fb.h"
#include "sys/include/subr_printk.h"
#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include <vt.h>
#include <serial.h>
#include <machdep.h>
#include <pci.h>
#include <pic.h>
#include <io/kbd/kbd.h>

// (2) 아키텍처별 halt
static void arch_halt_forever(void) {
#ifdef __x86_64__
    while (1) __asm__ volatile ("hlt");
#elif defined(__aarch64__)
    while (1) __asm__ volatile ("wfi");
#endif
}

vt_t terminal;

// 커널 진입점
void kmain(BootInfo* info) {

    init_amd64();
    serial_init();
    serial_write("Serial initialized!\n");
    

    // 프레임버퍼가 없으면 halt
    if (!info || !info->bi_framebuffer_addr ||
        info->bi_framebuffer_width == 0 || info->bi_framebuffer_height == 0) {
        arch_halt_forever();
    }

    set_vt(&terminal);
    vt_init(&terminal, info, 13, 0xFFFFFF, 0x000000);

    const unsigned char     /* UINT8 == unsigned char */ 
    expected_hash[32] = {
        0xEF, 0x92, 0xB7, 0x78, 0xBA, 0xFE, 0x77, 0x1E,
        0x89, 0x24, 0x5B, 0x89, 0xEC, 0xBC, 0x08, 0xA4,
        0x4A, 0x4E, 0x16, 0x6C, 0x06, 0x65, 0x99, 0x11,
        0x88, 0x1F, 0x38, 0x3D, 0x44, 0x73, 0xE9, 0x4F
    };

    if (memcmp(info->bi_pwd_hash, expected_hash, 32) != 0) {
        serial_write("Hash verification failed. System halt.\n");
        printk("Hash verification failed. System halt.\n");
        printk("ERROR: Password hash mismatch!\n");
    } else {
        serial_write("No Pash Problem!!\n");
        printk("No Pash Problem!!\n");
    }

    
    pci_init();
    pic_remap();
    asm volatile("sti");

    
    kbd_init();


    printk("GungYe!!!!\n");

    printk("\n\nG\nY\nN\nG\nY\nE\n");

    printk("\n%d", 12);
    

    arch_halt_forever();
}
