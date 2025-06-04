#include "pmap.h"
#include "paging.h"
#include "paging_asm.h"

void pmap_activate(pmap_t *pmap) {
    load_pml4(pmap->pml4_phys);
}