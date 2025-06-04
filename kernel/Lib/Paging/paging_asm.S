.section .text
.global load_pml4

# void load_pml4(uint64_t pml4_phys);
# rdi = pml4 테이블의 물리 주소

load_pml4:
    movq %rdi, %cr3   # CR3 레지스터에 PML4의 물리 주소를 설정
    ret