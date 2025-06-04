#pragma once
#include <stdint.h>
#include "../../Lib/boot/BootInfo.h"
#include "../fb/fb.h"
extern const uint8_t font8x8_basic[128][8];

int vt_draw_str(BootInfo *binfo, int x, int y, const char *str, uint32_t fg, uint32_t bg, int scale);
