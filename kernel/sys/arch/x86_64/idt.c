#include <stdint.h>
#include <string.h>
#include <isr.h>

#define IDT_ENTRY_COUNT 256

struct idt_entry {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t ist;        // IST index (0 if unused)
    uint8_t type_attr;  // Type and attributes (0x8E = interrupt gate)
    uint16_t offset_mid;
    uint32_t offset_high;
    uint32_t zero;
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));

// IDT 테이블 및 포인터
static struct idt_entry idt[IDT_ENTRY_COUNT];
static struct idt_ptr idtp;

// isr_stub0 ~ isr_stub31 주소가 들어 있는 테이블 (isr.S에서 제공)
extern void* isr_stubs[];
// irq1 handler import (isr.S에 있음)
extern void irq1_handler();

// IDT 엔트리 설정 함수
void idt_set_gate(int num, void* handler, uint16_t selector, uint8_t flags) {
    uint64_t addr = (uint64_t)handler;

    idt[num].offset_low  = addr & 0xFFFF;
    idt[num].selector    = selector;
    idt[num].ist         = 0;
    idt[num].type_attr   = flags;
    idt[num].offset_mid  = (addr >> 16) & 0xFFFF;
    idt[num].offset_high = (addr >> 32) & 0xFFFFFFFF;
    idt[num].zero        = 0;
}

// IDT 초기화
void idt_init() {
    memset(idt, 0, sizeof(idt));

    // 예외 핸들러 0~31 설정
    for (int i = 0; i < 32; i++) {
        idt_set_gate(i, isr_stubs[i], 0x08, 0x8E);  // CS = 0x08, type = interrupt
    }

    idt_set_gate(0x21, irq1_handler, 0x08, 0x8E);

    idtp.limit = sizeof(idt) - 1;
    idtp.base  = (uint64_t)&idt;

    // IDT 로드
    asm volatile("lidt %0" : : "m"(idtp));
}
