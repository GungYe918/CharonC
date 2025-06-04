/* linker.ld - aarch64 커널 링커 스크립트 (FreeBSD ABI 호환) */

OUTPUT_FORMAT("elf64-littleaarch64")
OUTPUT_ARCH(aarch64)
ENTRY(_start)

/* 메모리 레이아웃 정의 */
MEMORY
{
    /* FreeBSD ABI: 커널은 0x200000 (2MB)에 로드 */
    RAM : ORIGIN = 0x200000, LENGTH = 256M
}

/* 섹션 정의 */
SECTIONS
{
    /* FreeBSD 커널 시작 주소 - 0x200000 (2MB) */
    . = 0x200000;
    
    /* 코드 섹션 */
    .text ALIGN(4K) : {
        *(.text.boot)           /* 부트 코드 (locore.S의 _start) */
        *(.text)                /* 일반 코드 */
        *(.text.*)
    } > RAM
    
    /* 읽기 전용 데이터 */
    .rodata ALIGN(4K) : {
        *(.rodata)
        *(.rodata.*)
        *(.srodata)
        *(.srodata.*)
    } > RAM
    
    /* 초기화된 데이터 */
    .data ALIGN(4K) : {
        *(.data)
        *(.data.*)
        *(.sdata)
        *(.sdata.*)
    } > RAM
    
    /* BSS 섹션 시작 심볼 */
    .bss ALIGN(4K) : {
        __bss_start = .;
        *(.bss)
        *(.bss.*)
        *(.sbss)
        *(.sbss.*)
        *(COMMON)
        . = ALIGN(8);
        __bss_end = .;
    } > RAM
    
    /* 커널 끝 심볼 */
    . = ALIGN(4K);
    __kernel_end = .;
    
    /* 스택 섹션 (locore.S에서 정의) */
    .stack ALIGN(16) : {
        . += 0x4000;            /* 16KB 스택 */
    } > RAM
    
    /* 디버그 정보 (실행 파일에는 포함되지 않음) */
    .debug_info     0 : { *(.debug_info) }
    .debug_abbrev   0 : { *(.debug_abbrev) }
    .debug_line     0 : { *(.debug_line) }
    .debug_str      0 : { *(.debug_str) }
    .debug_ranges   0 : { *(.debug_ranges) }
    .debug_loc      0 : { *(.debug_loc) }
    
    /* 사용하지 않는 섹션 제거 */
    /DISCARD/ : {
        *(.note.*)
        *(.comment)
        *(.eh_frame)
        *(.eh_frame_hdr)
    }
}