ASM_SOURCES := $(wildcard bios/*.S)
C_SOURCES := $(wildcard bios/*.c)

OBJ := $(subst bios/,build/,$(C_SOURCES:.c=.o) $(ASM_SOURCES:.S=.o))

CC := riscv64-unknown-elf-gcc
CFLAGS := -Wall -Wextra -c -mcmodel=medany -ffreestanding -march=rv64id -mabi=lp64d 
AS := riscv64-unknown-elf-as
ASFLAGS := -march=rv64id -mabi=lp64d 
LD := riscv64-unknown-elf-ld 
LDFLAGS := -T link.ld -nostdlib# -L/usr/local/opt/llvm/lib/c++ -Wl,-rpath,/usr/local/opt/llvm/lib/c++
OBJCOPY := riscv64-unknown-elf-objcopy

all: AchieveBIOS.hex

AchieveBIOS.hex: build/AchieveBIOS.bin util/bintohex
	util/bintohex $< $@ 0x400

build/AchieveBIOS.bin: $(OBJ)
	$(LD) $(LDFLAGS) -o build/AchieveBIOS.elf $^ 
	cp build/AchieveBIOS.elf $@
	$(OBJCOPY) -O binary $@

build/%.o: bios/%.S
	-mkdir build
	$(AS) $(ASFLAGS) -o $@ $<

build/%.o: bios/%.c
	-mkdir build
	$(CC) $(CFLAGS) -c -o $@ $<

util/bintohex: util/bintohex.c
	gcc -o $@ $< -O2
