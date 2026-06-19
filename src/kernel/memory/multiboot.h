#ifndef MULTIBOOT_H
#define MULTIBOOT_H

#include <stdint.h>

#define MULTIBOOT_MAGIC 0xE85250D6 // Magic number for multiboot

// tag types
#define MB2_TAG_END       0
#define MB2_TAG_MMAP      6

// memory map entry types
#define MB2_MMAP_AVAILABLE  1
#define MB2_MMAP_RESERVED   2

// base multiboot2 info header
typedef struct {
    uint32_t total_size;
    uint32_t reserved;
} __attribute__((packed)) mb2_info_t;

// generic tag header
typedef struct {
    uint32_t type;
    uint32_t size;
} __attribute__((packed)) mb2_tag_t;

// memory map tag
typedef struct {
    uint32_t type;
    uint32_t size;
    uint32_t entry_size;
    uint32_t entry_version;
} __attribute__((packed)) mb2_tag_mmap_t;

// single memory map entry
typedef struct {
    uint64_t base_addr;
    uint64_t length;
    uint32_t type;
    uint32_t reserved;
} __attribute__((packed)) mb2_mmap_entry_t;


#endif // MULTIBOOT_H