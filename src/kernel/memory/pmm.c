// Physical Memory Manager (PMM) implementation for the kernel
#include <stdint.h>
#include "pmm.h"
#include "multiboot.h"
#include "../std/printf.h"

extern uint32_t _bss_end; // End of the BSS segment, defined in the linker script

static uint32_t* bitmap; // Bitmap to track free/used pages
static uint32_t bitmap_size; // Size of the bitmap in bytes or maximum number of pages
static uint32_t total_pages; // Total number of pages in the system
static uint32_t free_pages; // Number of free pages available

// bitmap helper functions
static inline void set_bit(uint32_t index) {
    bitmap[index / 32] |= (1 << (index % 32));
}

static inline void clear_bit(uint32_t index) {
    bitmap[index / 32] &= ~(1 << (index % 32));
}

static inline int test_bit(uint32_t index) {
    return (bitmap[index / 32] & (1 << (index % 32))) != 0;
}

// find highest ram address
static uint32_t find_highest_ram_address(uint32_t mb_info_addr) {
    mb2_info_t* mb_info = (mb2_info_t*)(uintptr_t)mb_info_addr;
    uint64_t highest_address = 0;

    // Iterate through the multiboot tags
    uint8_t* tag_ptr = (uint8_t*)mb_info + sizeof(mb2_info_t);
    while (tag_ptr < (uint8_t*)mb_info + mb_info->total_size) {
        mb2_tag_t* tag = (mb2_tag_t*)tag_ptr;

        if (tag->type == MB2_TAG_MMAP) {
            mb2_tag_mmap_t* mmap_tag = (mb2_tag_mmap_t*)tag;
            mb2_mmap_entry_t* entry = (mb2_mmap_entry_t*)(mmap_tag + 1);

            for (uint32_t i = 0; i < (mmap_tag->size - sizeof(mb2_tag_mmap_t)) / mmap_tag->entry_size; i++) {
                if (entry[i].type == MB2_MMAP_AVAILABLE) {
                    uint64_t end_address = entry[i].base_addr + entry[i].length;
                    if (end_address > highest_address) {
                        highest_address = end_address;
                    }
                }
            }
        }

        // Move to the next tag, ensuring alignment to 8 bytes
        tag_ptr += (tag->size + 7) & ~7;
    }

    return highest_address;
}

// mark available pages in bitmap
static void mark_available_pages(uint32_t mb_info_addr) {
    mb2_tag_t* tag_ptr = (mb2_tag_t*)((uintptr_t)mb_info_addr + sizeof(mb2_info_t));

    while (tag_ptr->type != MB2_TAG_END) {
        if (tag_ptr->type == MB2_TAG_MMAP) {
            mb2_tag_mmap_t* mmap_tag = (mb2_tag_mmap_t*)tag_ptr;
            mb2_mmap_entry_t* entry = (mb2_mmap_entry_t*)(mmap_tag + 1);

            for (uint32_t i = 0; i < (mmap_tag->size - sizeof(mb2_tag_mmap_t)) / mmap_tag->entry_size; i++) {
                if (entry[i].type == MB2_MMAP_AVAILABLE) {
                    uint64_t base_addr = entry[i].base_addr;
                    uint64_t length = entry[i].length;

                    // Mark pages in the bitmap
                    for (uint64_t addr = base_addr; addr < base_addr + length; addr += PAGE_SIZE) {
                        uint32_t page_index = addr / PAGE_SIZE;
                        if (page_index < total_pages) {
                            clear_bit(page_index); // Mark as free
                        }
                    }
                }
            }
        }

        // Move to the next tag, ensuring alignment to 8 bytes
        tag_ptr = (mb2_tag_t*)((uintptr_t)tag_ptr + ((tag_ptr->size + 7) & ~7));
    
    }
}

// mark reserved pages in bitmap
static void mark_reserved_pages(uint32_t mb_info_addr) {
    mb2_tag_t* tag_ptr = (mb2_tag_t*)((uintptr_t)mb_info_addr + sizeof(mb2_info_t));

    while (tag_ptr->type != MB2_TAG_END) {
        if (tag_ptr->type == MB2_TAG_MMAP) {
            mb2_tag_mmap_t* mmap_tag = (mb2_tag_mmap_t*)tag_ptr;
            mb2_mmap_entry_t* entry = (mb2_mmap_entry_t*)(mmap_tag + 1);

            for (uint32_t i = 0; i < (mmap_tag->size - sizeof(mb2_tag_mmap_t)) / mmap_tag->entry_size; i++) {
                if (entry[i].type == MB2_MMAP_RESERVED) {
                    uint64_t base_addr = entry[i].base_addr;
                    uint64_t length = entry[i].length;

                    // Mark pages in the bitmap
                    for (uint64_t addr = base_addr; addr < base_addr + length; addr += PAGE_SIZE) {
                        uint32_t page_index = addr / PAGE_SIZE;
                        if (page_index < total_pages) {
                            set_bit(page_index); // Mark as used
                        }
                    }
                }
            }
        }

        // Move to the next tag, ensuring alignment to 8 bytes
        tag_ptr = (mb2_tag_t*)((uintptr_t)tag_ptr + ((tag_ptr->size + 7) & ~7));
    }
}

void pmm_init(uint32_t mb_info_addr) {
    // read total_size BEFORE we place the bitmap
    mb2_info_t* mb_info = (mb2_info_t*)(uintptr_t)mb_info_addr;
    uint32_t mb_size = mb_info->total_size;

    uint64_t highest_address = find_highest_ram_address(mb_info_addr);
    total_pages = (uint32_t)(highest_address / PAGE_SIZE);
    bitmap_size = (total_pages + 31) / 32;

    // place bitmap AFTER both bss_end and multiboot info
    uint32_t bss_end     = (uint32_t)(uintptr_t)&_bss_end;
    uint32_t mb_info_end = mb_info_addr + mb_size;
    uint32_t safe_start  = (bss_end > mb_info_end) ? bss_end : mb_info_end;

    // align to 4 bytes
    safe_start = (safe_start + 3) & ~3;
    bitmap = (uint32_t*)(uintptr_t)safe_start;

    // now safe to initialize bitmap
    for (uint32_t i = 0; i < bitmap_size; i++)
        bitmap[i] = 0xFFFFFFFF;

    mark_available_pages(mb_info_addr);
    mark_reserved_pages(mb_info_addr);

    // reserve page 0
    set_bit(0);

    // reserve kernel + bitmap
    uint32_t bitmap_end_addr = (uint32_t)((uintptr_t)bitmap + bitmap_size * 4);
    for (uint32_t addr = 0x100000; addr < bitmap_end_addr; addr += PAGE_SIZE) {
        uint32_t page = addr / PAGE_SIZE;
        if (page < total_pages && !test_bit(page)) {
            set_bit(page);
        }
    }

    // count free pages
    free_pages = 0;
    for (uint32_t i = 0; i < total_pages; i++)
        if (!test_bit(i)) free_pages++;

}

void* pmm_alloc(void) {
    for (uint32_t i = 0; i < total_pages; i++) {
        if (!test_bit(i)) { // If the page is free
            set_bit(i); // Mark it as used
            free_pages--;
            return (void*)((uintptr_t)(i * PAGE_SIZE)); // Return the physical address of the allocated page
        }
    }
    return 0; // No free pages available
}

void pmm_free(void* page) {
    uint32_t page_index = (uintptr_t)page / PAGE_SIZE;
    if (page_index < total_pages && test_bit(page_index)) { // If the page is valid and currently used
        clear_bit(page_index); // Mark it as free
        free_pages++;
    }
}

uint32_t pmm_free_pages(void) {
    return free_pages;
}

uint32_t pmm_total_pages(void) {
    return total_pages;
}