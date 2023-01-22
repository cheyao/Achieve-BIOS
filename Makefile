ASM_SOURCES := $(wildcard src/*.S) $(wildcard util/libgcc/*.S)
C_SOURCES := $(wildcard src/*.c) $(wildcard util/libgcc/*.c)

ASM_OBJ := $(subst src/,build/,$(ASM_SOURCES:.S=.o))

CC := riscv64-unknown-elf-gcc
# CFLAGS := -Wall -Wextra -c -mcmodel=medany -ffreestanding -march=rv64id -mabi=lp64d -O2 -Wno-unused-variable -Wno-unused-but-set-variable -g

CFLAGS := -fno-omit-frame-pointer -O2 -nostdlib -ffreestanding -std=c17 -static -Wno-unused-parameter \
		  -Wno-unused-function -pedantic -Wall -Wextra -Wwrite-strings -Wstrict-prototypes -march=rv64i -mabi=lp64
AS := riscv64-unknown-elf-as
ASFLAGS := -march=rv64i -mabi=lp64
LD := riscv64-unknown-elf-ld 
LDFLAGS := -Wl,-Tlink.ld -Wl,-nostdlib
OBJCOPY := riscv64-unknown-elf-objcopy

all: AchieveBIOS.hex

AchieveBIOS.hex: $(C_SOURCES) $(ASM_SOURCES)
	@mkdir -p build 
	make AchieveBIOSreal.hex

AchieveBIOSreal.hex: build/AchieveBIOS.bin util/bintohex
	util/bintohex $< AchieveBIOS.hex 0x1fff

build/AchieveBIOS.bin: $(C_SOURCES)$(ASM_OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $(C_SOURCES) $(ASM_OBJ) -o build/AchieveBIOS.elf
	cp build/AchieveBIOS.elf $@
	$(OBJCOPY) -O binary $@

build/%.o: src/%.S
	$(AS) $(ASFLAGS) -o $@ $<

build/libgcc/%.o: util/libgcc/%.S
	$(CC) $(CFLAGS) -c -o $@ $<

util/bintohex: util/bintohex.c
	gcc -o $@ $< -O2

clean:
	@-rm -rf build AchieveBIOS.hex

dis: dissasemble
dissasemble: build/AchieveBIOS.bin
	riscv64-unknown-elf-objdump -d -S -l --visualize-jumps -M numeric,no-aliases --disassembler-color=color --inlines build/main.o
