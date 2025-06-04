#pragma once
#include <stdint.h>
#include "paging_asm.h"

typedef uint64_t paging_entry_t;

typedef struct {
    paging_entry_t entries[512];
} page_table_t __attribute__((aligned(0x1000)));

#define PAGE_PRESENT            0x001
#define PAGE_WRITABLE           0x002
#define PAGE_USER               0x004
#define PAGE_WRITE_THROUGH      0x008
#define PAGE_CACHE_DISABLE      0x010
#define PAGE_SIZE_2MB           0x080
#define PAGE_NX                 (1ULL << 63)

void paging_init();
uint64_t phys_to_directmap(uint64_t pa);
uint64_t directmap_to_phys(uint64_t va);

page_table_t *paging_get_pml4();
uint64_t paging_get_pml4_phys();