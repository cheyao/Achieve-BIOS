ASM_SOURCES := $(wildcard bios/*.S)
C_SOURCES := $(wildcard bios/*.c)

OBJ := $(subst bios/,build/,$(C_SOURCES:.c=.o) $(ASM_SOURCES:.S=.o))

CC := riscv64-unknown-elf-gcc
CFLAGS := -ffreestanding -march=rv64id
AS := riscv64-unknown-elf-as
ASFLAGS := -march=rv64id
LD := riscv64-unknown-elf-ld
LDFLAGS := -T link.ld -nostdlib
OBJCOPY := riscv64-unknown-elf-objcopy

all: AchieveBIOS.hex

AchieveBIOS.hex: AchieveBIOS.bin util/bintohex
	util/bintohex $< $@ 0x100000

AchieveBIOS.bin: $(OBJ)
	$(LD) $(LDFLAGS) -o $@ $^
	$(OBJCOPY) -O binary $@

build/%.o: bios/%.S
	-mkdir build
	$(AS) $(ASFLAGS) -o $@ $<

build/%.o: bios/%.c
	-mkdir build
	$(CC) $(CFLAGS) -c -o $@ $<

util/bintohex: util/bintohex.c
	gcc -o $@ $< -O2
