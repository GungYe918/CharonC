#include "fb.h"

int sibalba() {
    /*  UNSUPPORTED FORMAT  */
    return -1;
}

int fb_draw_pixel(BootInfo *binfo, int x, int y, uint32_t color24) {
    if (!binfo) return -1;

    uint32_t* fb = (uint32_t*)(uintptr_t)binfo->bi_framebuffer_addr;
    uint32_t pitch = (binfo->bi_framebuffer_pitch / 4);

    // 24비트 RGB값 분해 (RGB 순서 고정)
    uint8_t r = (color24 >> 16) & 0xFF;
    uint8_t g = (color24 >> 8) & 0xFF;
    uint8_t b = color24 & 0xFF;

    uint32_t color32 = 0;

    switch(binfo->bi_pixel_format) {
        case kPixelRGBResv8BitPerColor:
            // 0x00BBGGRR
            color32 = (b << 16) | (g << 8) | r;
            break;
        case kPixelBGRResv8BitPerColor:
            // 0x00RRGGBB
            color32 = (r << 16) | (g << 8) | b;
            break;
        default:
            sibalba();
            return -1;
    }

    fb[y * pitch + x] = color32;
    return 0;
}

int fb_draw_hline(BootInfo *binfo, int x1, int x2, int y, uint32_t color24) {
    if (!binfo) return -1;

    int width  = (int)binfo->bi_framebuffer_width;
    int height = (int)binfo->bi_framebuffer_height;

    // y가 화면 밖이면 무시
    if (y < 0 || y >= height) return 0;

    // x1~x2가 전체가 왼쪽/오른쪽 화면 밖이면 무시
    if (x2 <= 0 || x1 >= width) return 0;

    // x1, x2가 부분적으로만 벗어날 경우 무시
    if (x1 < 0) x1 = 0;
    if (x2 > width) x2 = width;

    for (int i = x1; i < x2; ++i) {
        fb_draw_pixel(binfo, i, y, color24);
    }


    return 0;
}



int fb_draw_vline(BootInfo *binfo, int y1, int y2, int x, uint32_t color24) {
    if (!binfo) return -1;

    int width  = (int)binfo->bi_framebuffer_width;
    int height = (int)binfo->bi_framebuffer_height;

    // x가 화면 바깥이면 무시
    if (x < 0 || x >= width) return 0;

    // y1~y2 전체가 화면 위/아래에 완전히 벗어나면 무시
    if (y2 <= 0 || y1 >= height) return 0;

    // y1이 음수거나 y2가 height를 넘으면 무시
    if (y1 < 0)    y1 = 0;
    if (y2 > height) y2 = height;

    for (int j = y1; j < y2; ++j) {
        fb_draw_pixel(binfo, x, j, color24);
    }


    return 0;
}

int fb_draw_rect(BootInfo* binfo, int x,  int y,  int w, int h, uint32_t color24) {
    if (!binfo) return -1;

    for (int j = 0; j < h; ++j) {
        for (int i = 0; i < w; ++i) {
            fb_draw_pixel(binfo, x + i, y + j, color24);
        }
    }

    return 0;
}

int fb_scen_clear(BootInfo *binfo, uint32_t color24) {
    if (!binfo) return -1;

    for (int j = 0; j < (int)(binfo->bi_framebuffer_height); ++j) {
        for (int i = 0; i < (int)(binfo->bi_framebuffer_width); ++i) {
            fb_draw_pixel(binfo, i, j, color24);
        }
    }

    return 0;
}