# My mac's llvm is here
export PATH := /usr/local/Cellar/llvm/15.0.7/bin:$(PATH)

ASM_SOURCES := $(wildcard src/*.S) $(wildcard util/libgcc/*.S)
C_SOURCES := $(wildcard src/*.c) $(wildcard util/libgcc/*.c)

OBJ := $(subst src/,build/,$(ASM_SOURCES:.S=.o) $(C_SOURCES:.c=.o))

CC := clang
CFLAGS := -fno-omit-frame-pointer -O2 -nostdlib -ffreestanding -std=c17 -static -Wno-unused-parameter --target=riscv64 \
		  -pedantic -Wall -Wextra -Wwrite-strings -Wstrict-prototypes -march=rv64i -mabi=lp64 -flto \
		  -I../AchieveOS/include -Wno-unused-function
AS := riscv64-unknown-elf-as
ASFLAGS := -march=rv64i -mabi=lp64
LD := ld.lld
LDFLAGS := -T link.ld -nostdlib
OBJCOPY := riscv64-unknown-elf-objcopy

all: AchieveBIOS.hex

AchieveBIOS.hex: $(C_SOURCES) $(ASM_SOURCES)
	@mkdir -p build 
	make -j16 AchieveBIOSreal.hex

AchieveBIOSreal.hex: build/AchieveBIOS.bin util/bintohex
	util/bintohex $< AchieveBIOS.hex 0x1fff

build/AchieveBIOS.bin: $(OBJ)
	$(LD) $(LDFLAGS) $^ -o build/AchieveBIOS.elf
	cp build/AchieveBIOS.elf $@
	$(OBJCOPY) -O binary $@

build/%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

build/%.o: src/%.S
	$(AS) $(ASFLAGS) -o $@ $<

build/libgcc/%.o: util/libgcc/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

build/libgcc/%.o: util/libgcc/%.S
	$(AS) $(CFLAGS) -o $@ $<

util/bintohex: util/bintohex.c
	gcc -o $@ $< -O2

clean:
	@-rm -rf build AchieveBIOS.hex

dis: dissasemble
dissasemble: build/AchieveBIOS.bin
	riscv64-unknown-elf-objdump -d -S -l --visualize-jumps -M numeric,no-aliases --disassembler-color=color --inlines build/main.o
