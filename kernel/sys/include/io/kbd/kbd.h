#pragma once
#include <stdint.h>

// 키보드 초기화
int init_kbd(void);
int kbd_handler(void);      // isr_handler상 33번에 등록

// 스캔코드에서 문자 하나 가져오기
char get_chr_from_scancode(uint8_t scode);

// 입력 버퍼에서 하나 가져오기
int kbd_read_char(void);