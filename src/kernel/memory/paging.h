#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>

// page directory/table entry flags
#define PAGE_PRESENT    (1 << 0)  // page is in memory
#define PAGE_WRITABLE   (1 << 1)  // page is writable
#define PAGE_USER       (1 << 2)  // accessible from user mode
#define PAGE_SIZE_4MB   (1 << 7)  // 4MB page (page directory only)

void paging_init(void);
void paging_map(uint32_t virt, uint32_t phys, uint32_t flags);

#endif