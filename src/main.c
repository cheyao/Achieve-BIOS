#include <stdint.h>
#include <stdbool.h>

#define UART 0xC0003

static inline void outd(uint32_t port, uint64_t val) {
    __asm__ __volatile__("sd %1, 0(%0)"::"r" (0xFFFFFFFF00000000 + port), "r" (val));
}

static inline void outw(uint32_t port, uint32_t val) {
    __asm__ __volatile__("sw %1, 0(%0)"::"r" (0xFFFFFFFF00000000 + port), "r" (val));
}

static inline void outh(uint32_t port, uint16_t val) {
    __asm__ __volatile__("sh %1, 0(%0)"::"r" (0xFFFFFFFF00000000 + port), "r" (val));
}

static inline void outb(uint32_t port, uint8_t val) {
    __asm__ __volatile__("sb %1, 0(%0)"::"r" (0xFFFFFFFF00000000 + port), "r" (val));
}

static inline uint64_t ind(uint32_t port) {
    uint64_t data;
    __asm__ __volatile__("ld %0, 0(%1)": "=r" (data):"r" (0xFFFFFFFF00000000 + port));
    return data;
}

static inline uint32_t inw(uint32_t port) {
    uint32_t data;
    __asm__ __volatile__("lw %0, 0(%1)": "=r" (data):"r" (0xFFFFFFFF00000000 + port));
    return data;
}

static inline uint16_t inh(uint32_t port) {
    uint16_t data;
    __asm__ __volatile__("lh %0, 0(%1)": "=r" (data):"r" (0xFFFFFFFF00000000 + port));
    return data;
}

static inline uint8_t inb(uint32_t port) {
    uint16_t data;
    __asm__ __volatile__("lb %0, 0(%1)": "=r" (data):"r" (0xFFFFFFFF00000000 + port));
    return data;
}

static void putchar(const char c) {
    outb(UART, c);
}

static void puts(const char *str) {
    for (int i = 0; str[i] != 0; i++)
        putchar(str[i]);
}

static void putc(const char *str, const long c) {
    for (int i = 0; i < c; i++)
        putchar(str[i]);
}

void main(void) {
    outd(0xC0001, 0x400); 
    inw(0xC0000);

    uint8_t  nl; // name length
    uint16_t is; // inode size
    uint32_t bs, itaddr, ioff; // block count, inode count, block size, inode table address, root dir inode offset
    char name[256];

    outd(0xC0001, 1024 + 24); 
    bs = inw(0xC0000);
    bs = 1024 << bs;

    outd(0xC0001, 1024 + 88); 
    is = inh(0xC0000);

    outd(0xC0001, (bs == 1024 ? bs * 2 : bs) + 8);  //  Block Group Descriptor Table 
    itaddr = inw(0xC0000);

    outd(0xC0001, itaddr * bs + is + 40); // Inode table 2nd entry - root dir
    ioff = inw(0xC0000);

    outd(0xC0001, ioff * bs + 6); // Root dir
    nl = inb(0xC0000);
    inb(0xC0000);
    for (int i = 0; i < nl; i++)
        name[i] = inb(0xC0000);
    
    puts("Hello UART world!\n");
    puts("Dir name:");
    putc(name, nl);
    putchar('\n');
}

    /*
    uint64_t t64;

    printf("disk has %#x blocks and %#x inodes, block size = %#x, total disk size = %#x\n", bc, ic, bs, bc * bs); // Good
    */
