#include "paging.h"
#include <stdint.h>
#include <stddef.h>

// locore.S에서 정의된 정적 페이지 테이블
extern page_table_t pml4;
extern page_table_t pdpt;
extern page_table_t pd;
extern page_table_t pdpt_direct;
extern page_table_t pd_direct;

#define KERNEL_PHYS_BASE   0x00200000ULL
#define KERNEL_VIRT_BASE   0xFFFFFFFF80200000ULL
#define DIRECTMAP_BASE     0xFFFF800000000000ULL

#define PAGE_PRESENT        0x001
#define PAGE_WRITABLE       0x002
#define PAGE_SIZE_2MB       0x080
#define PAGE_FLAGS          (PAGE_PRESENT | PAGE_WRITABLE)
#define PAGE_2MB_FLAGS      (PAGE_FLAGS | PAGE_SIZE_2MB)

static page_table_t* current_pml4 = &pml4;
static uint64_t current_pml4_phys = (uint64_t)&pml4;

void paging_init() {
    // 커널 매핑
    size_t pml4_index = (KERNEL_VIRT_BASE >> 39) & 0x1FF;  // = 511
    size_t pdpt_index = (KERNEL_VIRT_BASE >> 30) & 0x1FF;  // = 0
    size_t pd_index   = (KERNEL_VIRT_BASE >> 21) & 0x1FF;  // = 1

    pd.entries[pd_index] = KERNEL_PHYS_BASE | PAGE_2MB_FLAGS;
    pdpt.entries[pdpt_index] = (uint64_t)&pd | PAGE_FLAGS;
    pml4.entries[pml4_index] = (uint64_t)&pdpt | PAGE_FLAGS;

    // Direct map: PML4[256] → PDPT → PD
    for (size_t i = 0; i < 512; ++i) {
        pd_direct.entries[i] = (i * 0x200000ULL) | PAGE_2MB_FLAGS;
    }
    pdpt_direct.entries[0] = (uint64_t)&pd_direct | PAGE_FLAGS;
    pml4.entries[256] = (uint64_t)&pdpt_direct | PAGE_FLAGS;

    // CR3에 PML4 물리 주소 로드
    current_pml4 = &pml4;
    current_pml4_phys = (uint64_t)&pml4;
    __asm__ volatile("mov %0, %%cr3" :: "r"(current_pml4_phys));
}

page_table_t* paging_get_pml4() {
    return current_pml4;
}

uint64_t paging_get_pml4_phys() {
    return current_pml4_phys;
}

uint64_t phys_to_directmap(uint64_t pa) {
    return pa + DIRECTMAP_BASE;
}

uint64_t directmap_to_phys(uint64_t va) {
    return va - DIRECTMAP_BASE;
}
