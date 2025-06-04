OUTPUT_FORMAT("elf64-x86-64")
OUTPUT_ARCH(i386:x86-64)
ENTRY(_start)

PHDRS
{
  text PT_LOAD FLAGS(5); /* R + X */
  data PT_LOAD FLAGS(6); /* R + W */
  bss  PT_LOAD FLAGS(6); /* R + W */
}

SECTIONS
{
  . = 0x200000;  /* FreeBSD 커널 고정 가상 주소 */

  /* 코드 + 읽기 전용 데이터 → text segment */
  .text : ALIGN(0x1000)
  {
    KEEP(*(.multiboot))         /* multiboot header if any */
    *(.text .text.*)
  } :text

  .rodata : ALIGN(0x1000)
  {
    *(.rodata .rodata.*)
  } :text

  /* 초기화된 데이터 → data segment */
  .data : ALIGN(0x1000)
  {
    *(.data .data.*)
  } :data

  /* 비초기화 데이터 → bss segment */
  .bss : ALIGN(0x1000)
  {
    __bss_start = .;
    *(.bss .bss.* COMMON)
    __bss_end = .;
  } :bss

  /* 무시할 디버깅/주석 정보 */
  /DISCARD/ : {
    *(.eh_frame) *(.note*) *(.comment)
  }
}