#include "../fb/fb.h"
#include <vt.h>
#include <string.h>

static vt_t *g_vt = NULL;

// 폰트 기본 크기 정의 (8x8 비트맵 폰트)
#define FONT_WIDTH  8
#define FONT_HEIGHT 8

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

int set_vt(vt_t *vt) {
    if (!vt) return -1;
    g_vt = vt;

    return 0;
}

vt_t* get_vt(void) {
    return g_vt;
}

int vt_scroll(vt_t* vt) {
    if (!vt) return -1;
    uint32_t* fb = (uint32_t*)(uintptr_t)vt->binfo->bi_framebuffer_addr;
    int pitch = vt->binfo->bi_framebuffer_pitch / 4;
    int width = vt->binfo->bi_framebuffer_width;
    int height = vt->binfo->bi_framebuffer_height;

    // 한 줄 높이 = scale 값 (문자 간격) + 추가 여백
    int line_height = vt->scale + 4;

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

int vt_putc(vt_t* vt, const char c) {
    if (!vt || !vt->binfo) return -1;

    // 문자 간격은 scale 사용, 지울 때는 실제 렌더링 크기 고려
    int char_width = vt->scale;
    int char_height = vt->scale;
    // 실제 렌더링되는 폰트 크기 (for erasing)
    int actual_char_width = FONT_WIDTH * vt->scale;
    int actual_char_height = FONT_HEIGHT * vt->scale;
    const int tab_width = 4;

    switch (c) {
        case '\n' :
            vt->cursor_x = 0;
            vt->cursor_y += char_height + 4;
            break;
        
        case '\t' :
            {
                int spaces = tab_width - (vt->cursor_x / char_width) % tab_width;
                for (int i = 0; i < spaces; ++i)
                    vt_putc(vt, ' ');
            }
            break;

        case '\r' :
            vt->cursor_x = 0;
            break;
        
        case '\b':
            if (vt->cursor_x >= char_width) {
                // 같은 줄에서 한 문자 뒤로
                vt->cursor_x -= char_width;
                
                // 현재 커서 위치의 문자만 지움 (적당한 크기로)
                for (int dy = 0; dy < char_height + 4; ++dy) {
                    for (int dx = 0; dx < char_width; ++dx) {
                        if (vt->cursor_x + dx < (int)vt->binfo->bi_framebuffer_width &&
                            vt->cursor_y + dy < (int)vt->binfo->bi_framebuffer_height) {
                            fb_draw_pixel(vt->binfo, vt->cursor_x + dx, vt->cursor_y + dy, vt->bg_color);
                        }
                    }
                }
            } else if (vt->cursor_y >= char_height + 4) {
                // 이전 줄 끝으로 이동
                vt->cursor_y -= (char_height + 4);
                // 화면 너비를 문자 단위로 나눈 후 마지막 문자 위치로 설정
                int chars_per_line = (int)vt->binfo->bi_framebuffer_width / char_width;
                vt->cursor_x = (chars_per_line - 1) * char_width;
                
                // 현재 커서 위치의 문자만 지움 (경계 체크 포함)
                for (int dy = 0; dy < char_height + 4; ++dy) {
                    for (int dx = 0; dx < char_width; ++dx) {
                        if (vt->cursor_x + dx < (int)vt->binfo->bi_framebuffer_width &&
                            vt->cursor_y + dy < (int)vt->binfo->bi_framebuffer_height) {
                            fb_draw_pixel(vt->binfo, vt->cursor_x + dx, vt->cursor_y + dy, vt->bg_color);
                        }
                    }
                }
            }
            break;
        
        default :
            vt_draw_char(vt, vt->cursor_x, vt->cursor_y, c);
            vt->cursor_x += char_width;

            if (vt->cursor_x + char_width >= (int)vt->binfo->bi_framebuffer_width) {
                vt->cursor_x = 0;
                vt->cursor_y += char_height + 4;
            }
            
            break;
    }

    // 스크롤 필요 여부 체크
    if (vt->cursor_y + char_height + 4 >= (int)vt->binfo->bi_framebuffer_height) {
        vt_scroll(vt);
    }

    return 0;
}

int vt_puts(vt_t* vt, const char* str) {
    if (!vt || !vt->binfo) return -1;

    // 문자 간격은 scale 사용
    int char_width = vt->scale;
    int char_height = vt->scale;
    const int tab_width = 4;

    while (*str) {
        char c = *str++;
        switch(c) {
            case '\n':
                vt->cursor_x = 0;
                vt->cursor_y += char_height + 4;

                // 스크롤 가능 여부 체크
                if (vt->cursor_y + char_height + 4 >= (int)vt->binfo->bi_framebuffer_height) {
                    vt_scroll(vt);
                }
                break;
            
            case '\t':
                {
                    int spaces = tab_width - (vt->cursor_x / char_width) % tab_width;
                    for (int i = 0; i < spaces; ++i)
                        vt_putc(vt, ' ');
                }
                break;
            
            default:
                vt_putc(vt, c);
                break;
        }
    }
    
    return 0;
}