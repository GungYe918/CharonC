#include "vt.h"
#include "../fb/fb.h"
#include <string.h>


int vt_init(vt_t* vt, BootInfo* binfo, int font_scale, uint32_t fg, uint32_t bg) {
    if (!binfo || !vt) return -1;
    
    // boot_info && font_scale && fg/bg 초기화
    vt->binfo = binfo;
    vt->scale = font_scale;
    vt->fg_color = fg;
    vt->bg_color = bg;

    // cursor 위치 초기화
    vt->cursor_x = 0;
    vt->cursor_y = 0;

    fb_scen_clear(binfo, bg);
    return 0;
}

int vt_scroll(vt_t* vt) {
    if (!vt) return -1;
    uint32_t* fb = (uint32_t*)(uintptr_t)vt->binfo->bi_framebuffer_addr;
    int pitch = vt->binfo->bi_framebuffer_pitch / 4;
    int width = vt->binfo->bi_framebuffer_width;
    int height = vt->binfo->bi_framebuffer_height;

    int line_height = vt->scale;


    // 한 줄 위로 복사
    for (int y = 0; y < height - line_height; ++y) {
        for (int x = 0; x < width; ++x) {
            fb[y * pitch + x] = fb[(y + line_height) * pitch + x];
        }
    }

    // 마지막 줄은 배경색으로 초기화
    for (int y = height - line_height; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            fb[y * pitch + x] = vt->bg_color;
        }
    }

    vt->cursor_y -= line_height;
    if (vt->cursor_y < 0) vt->cursor_y = 0;

    return 0;
}

int vt_putc(vt_t* vt, char c) {
    if (!vt) return -1;

    int char_width = vt->scale;
    int char_height = vt->scale;

    vt_draw_char(vt, vt->cursor_x, vt->cursor_y, c);
    vt->cursor_x += char_width;

    if (vt->cursor_x + char_width >= (int)vt->binfo->bi_framebuffer_width) {
        vt->cursor_x = 0;
        vt->cursor_y += char_height;
    }

    if (vt->cursor_y + char_height >= (int)vt->binfo->bi_framebuffer_height) {
        vt_scroll(vt);
    }

    return 0;
}

int vt_puts(vt_t* vt, char* str) {
    if (!vt || !vt->binfo) return -1;

    const int tab_width = 4;

    while (*str) {

        char c = *str++;
        switch(c) {
            case '\n':
                vt->cursor_x = 0;
                vt->cursor_y += vt->scale;

                // 스크롤 가능 여부 체크
                if (vt->cursor_y + vt->scale >= (int)vt->binfo->bi_framebuffer_height) {
                    vt_scroll(vt);
                }
                break;
            
            case '\t':
                int spaces = tab_width - (vt->cursor_x / vt->scale) % tab_width;
                for (int i = 0; i < spaces; ++i)
                    vt_putc(vt, ' ');
                break;
            
            default:
                vt_putc(vt, c);
                break;

        }
    }
    
    return 0;
}
