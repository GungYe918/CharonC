#pragma once
#include <stdint.h>
#include "../../Lib/boot/BootInfo.h"
#include "../dev/fb/fb.h"
extern const uint8_t font8x8_basic[128][8];

// vt 콘솔 정의 구조체
typedef struct vt {
    BootInfo* binfo;        // BootInfo
    int cursor_x;           // cursor point(x) 
    int cursor_y;           // cursor point(y)
    int scale;              // 스케일 수치 (폰트)
    uint32_t  fg_color;     // 전경 색상
    uint32_t  bg_color;     // 배경 색상
    uint32_t  __reserved;   // padding (8B align 유지)
} __attribute__((aligned(8))) vt_t;

// vt 초기화
int vt_init(vt_t* vt, BootInfo* binfo, int font_scale, uint32_t fg, uint32_t bg);

// vt 세팅하기
int set_vt(vt_t *vt);

// g_vt 가져오기
vt_t* get_vt(void);


/**
 * Vt Utils
 */
// vt - char 출력
int vt_draw_char(vt_t *vt, int x, int y, char c);

// vt - 문자열 출력
int vt_putc(vt_t* vt, const char c);
int vt_puts(vt_t* vt, const char* str);