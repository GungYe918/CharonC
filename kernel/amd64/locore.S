# locore.S - x86_64 커널 진입점 (UEFI용)
# BootInfo 포인터는 RDI로 전달됨

.section .text
.code64
.global _start
.extern kmain

# GDT: null, code, data
.section .data
.align 16
gdt64:
    .quad 0x0000000000000000        # 0x00: NULL
    .quad 0x00AF9A000000FFFF        # 0x08: 64bit code (base=0, limit=4GB, G=1, L=1)
    .quad 0x00AF92000000FFFF        # 0x10: 64bit data

gdt64_desc:
    .word (3*8)-1                   # limit = 3 entries * 8 - 1
    .quad gdt64                     # base  = &gdt64

.section .text
.align 16
_start:
    cli
    // lgdt gdt64_desc  <-- 이전 코드
    movabs $gdt64_desc, %rax  // gdt64_desc의 절대 주소를 RAX에 로드
    lgdt (%rax)               // RAX 레지스터가 가리키는 메모리에서 GDT 디스크립터 로드

    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %ss
    mov %ax, %fs  // FS, GS도 동일하게 초기화 (objdump 결과 반영)
    mov %ax, %gs

    lea next(%rip), %rax
    pushq $0x08
    pushq %rax
    lretq

next:
    # 4. 스택 초기화
    movq $stack_top, %rsp
    andq $-16, %rsp                 # 16바이트 정렬 (SSE ABI 등 호환)
    # BootInfo 포인터(RDI)는 이미 세팅되어 있음

    # 5. C 커널 메인 호출
    call kmain

1:  hlt                             # 만약 kmain이 return하면 무한 루프
    jmp 1b

# ----- 스택 공간 -----
.section .bss
.align 16
stack_bottom:
    .skip 0x4000                    # 16KB
stack_top:
