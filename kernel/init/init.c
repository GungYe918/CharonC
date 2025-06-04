#include "Lib/boot/BootInfo.h"
#include "sys/dev/fb/fb.h"
#include "sys/include/subr_printk.h"
#include <stdint.h>
#include <stddef.h>
#include <vt.h>
#include <serial.h>
#include <machdep.h>

/*
static void lidt_dummy(void) {
#ifdef __x86_64__
    static uint8_t dummy_idt[16] = {0};
    struct {
        uint16_t limit;
        uint64_t base;
    } __attribute__((packed)) idtr = {
        .limit = sizeof(dummy_idt) - 1,
        .base  = (uintptr_t)dummy_idt,
    };
    __asm__ volatile ("lidt %0" : : "m"(idtr));
#elif defined(__aarch64__)
    // ARM64에는 IDT 개념이 없음 → 아무 작업 안 함
#endif
}
*/

// 화면에 픽셀 쓰기 (BGRA)
void draw_color_bars(BootInfo* bootinfo) {
    if (!bootinfo) return;

    uint32_t width  = bootinfo->bi_framebuffer_width;
    uint32_t height = bootinfo->bi_framebuffer_height;

    for (uint32_t y = 0; y < height; y++) {
        uint32_t color = 0;
        if (y < height / 3) {
            // 갈색 (Brown): R=165, G=42, B=42
            color = 0xA52A2A;
        }
        else if (y < (height * 2) / 3) {
            // 핑크 (Pink): R=255, G=192, B=203
            color = 0xFFC0CB;
        }
        else {
            // 노란색 (Yellow): R=255, G=255, B=0
            color = 0xFFFF00;
        }

        for (uint32_t x = 0; x < width; x++) {
            fb_draw_pixel(bootinfo, x, y, color);
        }
    }
}

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


    draw_color_bars(info);

    fb_draw_hline(info, 0, 400, 300, 0xFFFFFF);
    fb_draw_vline(info, 0, 400, 300, 0xFFFFFF);

    set_vt(&terminal);
    vt_init(&terminal, info, 13, 0xFFFFFF, 0x000000);
    printk("GungYe!!!!\n");

    printk("\n\nG\nY\nN\nG\nY\nE\n");

    printk("\n%d", 12);
    

    arch_halt_forever();
}
