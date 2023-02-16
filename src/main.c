/*
 * @brief Minimal
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <kernel/ports.h>

#define BLOCK_SIZE 4096

static inline void putchar(const char c) {
    outb(UART, c);
}

#define PANIC(str) do { puts(str); while (1); } while(0);

typedef struct pghr {
    uint32_t p_type; /* Type of segment */
    uint32_t p_flags; /* Segment attributes */
    uintptr_t p_offset; /* Offset in file */
    uintptr_t p_vaddr; /* Virtual address in memory */
    uintptr_t p_paddr; /* Reserved */
    uint64_t p_filesz; /* Size of segment in file */
    uint64_t p_memsz; /* Size of segment in memory */
    uint64_t p_align; /* Alignment of segment */
} pghr;

static inline void puts(const char *restrict str) {
    for (int i = 0; str[i] != 0; i++)
        putchar(str[i]);

    putchar('\n');
}

static inline int strcmp(const char*restrict lhs, const char*restrict rhs) {
    for (uint64_t i = 0;; i++) {
        if (rhs[i] == 0 || lhs[i] == 0)
            return 1;

        if (rhs[i] != lhs[i]) 
            return 0;
    }
}

#define seek(addr) outd(SD_SEEK, addr)

uint32_t inode_size;
uint32_t inode_table_address;

uint32_t get_dir_inode(uint32_t parent_inode, const char*restrict name) {
    uint8_t name_length;
    char name_buffer[256];
    uint32_t block_pointers[15];

    seek(inode_table_address + ((parent_inode - 1) * inode_size) / BLOCK_SIZE);

    for (int i = 0; i < 15; i++) 
        block_pointers[i] = inw(SD_DATA + (parent_inode - 1) * inode_size + 40 + i * 4);

    for (int i = 0; i < 13; i++) {
        if (block_pointers[i] == 0)
            break;

        seek(block_pointers[i]);

        for (int directory_pointer = 0;;) {
            directory_pointer += inh(SD_DATA + directory_pointer + 4);
            name_length = inb(SD_DATA + directory_pointer + 6);

            int j;
            for (j = 0; j < name_length; j++) {
                name_buffer[j] = inb(SD_DATA + directory_pointer + 8 + j);
            }
            name_buffer[j] = 0;

            puts(name_buffer);

            if (strcmp(name_buffer, name))
                return inw(SD_DATA + directory_pointer);

            if (directory_pointer % BLOCK_SIZE == 0)
                break;
        }
    }

    puts(name); // Too lazy to write a strcat
    PANIC(" not found!");
}

void block_to_mems(uint64_t number, uint64_t block, uint64_t mem) {
    for (int i = 0; i < number; i++) {
        seek(block + i);
        for (uint64_t j = 0; j < 0x1000; j += 4)
            *((uint64_t *) mem + j + i * BLOCK_SIZE) = ind(SD_DATA + j);
    }
}

uint32_t getblock(uint32_t inode, uint64_t number) {
    return ;
}

uintptr_t readelf(uint32_t inode) {
    // Here comes the tough stuff :(
    seek(inode_table_address + ((inode - 1) * inode_size) / BLOCK_SIZE);

    seek(inw(SD_DATA + (inode - 1) * inode_size + 40)); // Go to block 0 of the file

    // Check for elf header  \/ = 0x7F E L F
    if (inw(SD_DATA) == 0x464C457F) {
        puts("kernel recognised");

        uintptr_t entry = ind(0x18), phoff = ind(0x20);
        uint16_t phnum = inh(0x38);

        block_to_mem(phoff % BLOCK_SIZE, inq(MEM_SIZE));

        pghr hr = phoff / BLOCK_SIZE;

        return entry;
    } else /* if not elf */ {
        PANIC("kernel is not elf!");
    }
}

uintptr_t main(void) {
    puts("Achieve BIOS (Pre-Alpha)");

    seek(0);
    inode_size = inw(SD_DATA + 1024 + 88);

    // Block group
    seek(1);
    inode_table_address = inw(SD_DATA + 8);

    return readelf(get_dir_inode(get_dir_inode(get_dir_inode(get_dir_inode(2, "System"), "Library"), "Kernel"), "kernel"));
}
