// Physical Memory Manager (PMM) implementation for the kernel

#ifndef PMM_H
#define PMM_H

#include <stdint.h>

#define PAGE_SIZE 4096 // 4KB pages

// Physical Memory Manager (PMM) interface for the kernel
void     pmm_init(uint32_t mb_info_addr);
void*    pmm_alloc(void);   // allocate one 4KB page
void     pmm_free(void* page);
uint32_t pmm_free_pages(void);
uint32_t pmm_total_pages(void);

#endif // PMM_H