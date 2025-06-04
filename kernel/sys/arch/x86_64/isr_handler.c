#include <isr.h>

// 예외 메시지 리스트
static const char* exception_messages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",
    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating-Point Exception",
    "Virtualization Exception",
    "Control Protection Exception",
    "Reserved (22)",
    "Reserved (23)",
    "Reserved (24)",
    "Reserved (25)",
    "Reserved (26)",
    "Reserved (27)",
    "Hypervisor Injection Exception",
    "VMM Communication Exception",
    "Security Exception",
    "Reserved (31)"
};


struct isr_frame {
    uint64_t r15, r14, r13, r12, r11, r10, r9, r8;
    uint64_t rsi, rdi, rbp, rdx, rcx, rbx, rax;
    uint64_t int_no;
    uint64_t err_code;
    uint64_t rip, cs, rflags, rsp, ss;
} __attribute__((packed));

void isr_handler_c(isr_frame_t* frame) {
    printk("\n[EXCEPTION] #%d - %s\n", (int)frame->int_no,
           (frame->int_no < 32 ? exception_messages[frame->int_no] : "Unknown"));
    printk("  RIP = %lx  ERR = %lx\n", frame->rip, frame->err_code);

    if (frame->int_no == 0) {  // Divide by Zero
        printk("Handled divide-by-zero! Skipping instruction.\n");
        frame->rip += 2;  // div r/m64는 보통 2~3바이트지만, 임시로 2byte 이동 (정확히는 분석 필요)
        return;
    }

    // 기타 예외 → halt
    while (1) __asm__ volatile ("cli; hlt");
}