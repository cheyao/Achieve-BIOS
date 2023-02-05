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

static inline void puts(const char *restrict str) {
    for (int i = 0; str[i] != 0; i++)
        putchar(str[i]);
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

void block_to_mem(uint64_t block, uint64_t mem) {
    seek(block);
    for (int i = 0; i < 0x1000; i += 4)
        *((uint64_t *) mem + i) = ind(SD_DATA + i);
}

uintptr_t readelf(uint32_t inode) {
    // Here comes the tough stuff :(
    seek(inode_table_address + ((inode - 1) * inode_size) / BLOCK_SIZE);

    seek(inw(SD_DATA + (inode - 1) * inode_size + 40)); // Go to block 0
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
