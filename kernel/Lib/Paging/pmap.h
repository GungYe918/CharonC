//pmap.h
#pragma once
#include <stdint.h>
#include "paging.h"

typedef struct {
    page_table_t *pml4_table; 
    uint64_t      pml4_phys;
} pmap_t;

void pmap_activate(pmap_t *pmap);