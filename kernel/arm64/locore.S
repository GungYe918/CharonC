# locore.S - aarch64 커널 진입점 (UEFI용)
# BootInfo 포인터는 X0로 전달됨

.section .text
.global _start
.extern kmain

_start:
    // 1. 인터럽트 비활성화
    msr daifset, #0xf               // Debug, SError, IRQ, FIQ 모두 마스크

    // 2. 예외 레벨 확인 및 설정
    mrs x1, currentel               // 현재 예외 레벨 읽기
    and x1, x1, #0xc               // EL 필드만 추출 (bit[3:2])
    cmp x1, #0x8                   // EL2인지 확인
    b.eq setup_el2
    cmp x1, #0xc                   // EL3인지 확인  
    b.eq setup_el3
    b setup_el1                    // EL1이면 바로 EL1 설정으로

setup_el3:
    // EL3에서 EL1으로 전환 준비
    mov x1, #0x5                   // RW=1 (AArch64), NS=1 (Non-secure)
    msr scr_el3, x1
    
    mov x1, #0x3c9                 // EL1h 모드, 모든 인터럽트 마스크
    msr spsr_el3, x1
    
    adr x1, setup_el1
    msr elr_el3, x1
    eret                           // EL1으로 전환

setup_el2:
    // EL2에서 EL1으로 전환 준비
    mov x1, #0x80000000            // RW=1 (AArch64 for EL1)
    msr hcr_el2, x1
    
    mov x1, #0x3c5                 // EL1h 모드, 모든 인터럽트 마스크
    msr spsr_el2, x1
    
    adr x1, setup_el1
    msr elr_el2, x1
    eret                           // EL1으로 전환

setup_el1:
    // 3. MMU 및 캐시 설정 (기본적으로 비활성화)
    mrs x1, sctlr_el1
    bic x1, x1, #0x1               // MMU 비활성화
    bic x1, x1, #0x4               // 데이터 캐시 비활성화
    bic x1, x1, #0x1000            // 명령어 캐시 비활성화
    bic x1, x1, #0x800             // 분기 예측 비활성화
    orr x1, x1, #0x2               // 정렬 체크 활성화
    msr sctlr_el1, x1
    isb                            // 명령어 동기화 배리어

    // 4. 스택 포인터 설정
    adrp x1, stack_top             // 스택 상단 주소의 페이지 부분
    add x1, x1, :lo12:stack_top    // 하위 12비트 추가
    mov sp, x1                     // 스택 포인터 설정
    
    // 스택 16바이트 정렬 (AAPCS64 호출 규약)
    // SP 레지스터는 직접 비트 연산할 수 없으므로 임시 레지스터 사용
    mov x1, sp                     // SP를 x1으로 복사
    and x1, x1, #0xfffffffffffffff0  // 16바이트 정렬 마스크 적용
    mov sp, x1                     // 정렬된 값을 다시 SP에 저장

    // 5. BSS 섹션 초기화 (선택적)
    adrp x1, __bss_start
    add x1, x1, :lo12:__bss_start
    adrp x2, __bss_end  
    add x2, x2, :lo12:__bss_end
    
clear_bss:
    cmp x1, x2
    b.ge bss_cleared
    str xzr, [x1], #8              // 8바이트씩 0으로 초기화
    b clear_bss

bss_cleared:
    // 6. BootInfo 포인터는 이미 X0에 설정되어 있음
    // UEFI에서 전달된 매개변수 유지

    // 7. C 커널 메인 호출
    bl kmain

    // 8. kmain이 반환되면 무한 루프
1:  wfi                            // Wait For Interrupt
    b 1b

# ----- 스택 공간 -----
.section .bss
.align 16
stack_bottom:
    .skip 0x4000                   // 16KB
stack_top:

# BSS 섹션 심볼 (링커 스크립트에서 정의되어야 함)
.weak __bss_start
.weak __bss_end