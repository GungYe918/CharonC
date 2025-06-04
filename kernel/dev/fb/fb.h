#pragma once
#include "../../Lib/boot/BootInfo.h"
#include <stdint.h>
#include <stddef.h>

#define COLOR_RGB 0
#define COLOR_BGR 1

#define RGB(r, g, b)  (((r) << 16) | ((g) << 8) | (b))
#define BGR(r, g, b)  (((b) << 16) | ((g) << 8) | (r))


int fb_draw_pixel(BootInfo *binfo, int x,  int y,                uint32_t color24);
int fb_draw_hline(BootInfo *binfo, int x1, int x2, int y,        uint32_t color24);
int fb_draw_vline(BootInfo *binfo, int y1, int y2, int x,        uint32_t color24);
int fb_draw_rect (BootInfo* binfo, int x,  int y,  int w, int h, uint32_t color24);
int fb_scen_clear(BootInfo *binfo, uint32_t color24);
