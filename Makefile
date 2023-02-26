# My mac's llvm is here
export PATH := /usr/local/Cellar/llvm/15.0.7/bin:$(PATH)

ASM_SOURCES := $(wildcard src/*.S) $(wildcard util/libgcc/*.S)
C_SOURCES := $(wildcard src/*.c) $(wildcard util/libgcc/*.c)

OBJ := $(subst src/,build/,$(ASM_SOURCES:.S=.o) $(C_SOURCES:.c=.o))

CC := clang
CFLAGS := -fno-omit-frame-pointer -Os -nostdlib -ffreestanding -std=c2x -static --target=riscv64 \
		  -pedantic -Wall -Wextra -Wwrite-strings -Wstrict-prototypes -march=rv64i -mabi=lp64 -flto -fno-builtin \
		  -I../AchieveOS/include -fno-stack-protector -nodefaultlibs \
		  -fms-extensions -fno-PIC
AS := clang
ASFLAGS := --target=riscv64 -march=rv64i -mabi=lp64 -nostdlib
LD := ld.lld
LDFLAGS := -T link.ld -nostdlib
OBJCOPY := riscv64-unknown-elf-objcopy
CT := clang-tidy

.PHONY: all dis clang-tidy clean dissasemble

all: AchieveBIOS.hex

AchieveBIOS.hex: $(C_SOURCES) $(ASM_SOURCES)
	@mkdir -p build
	@mkdir -p build/tests
	make AchieveBIOSreal.hex

AchieveBIOSreal.hex: build/AchieveBIOS.bin util/bintohex
	util/bintohex $< AchieveBIOS.hex 0x1fff

build/AchieveBIOS.bin: $(OBJ)
	$(LD) $(LDFLAGS) $^ -o build/AchieveBIOS.elf
	cp build/AchieveBIOS.elf $@
	$(OBJCOPY) -O binary $@

build/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

build/%.o: src/%.S
	$(AS) $(ASFLAGS) -c $< -o $@

build/libgcc/%.o: util/libgcc/%.c
	$(CC) $(CFLAGS) -c $< -o $@

build/libgcc/%.o: util/libgcc/%.S
	$(AS) $(CFLAGS) -c $< -o $@

util/bintohex: util/bintohex.c
	gcc -o $@ $< -O2

clean:
	@-rm -rf build AchieveBIOS.hex

clang-tidy: $(C_SOURCES)
	-@$(CT) $^ --system-headers --checks=*,-altera-struct-pack-align,-altera-id-dependent-backward-branch,-cppcoreguidelines-*,-readability-magic-numbers,-cppcoreguidelines-avoid-magic-numbers,-hicpp-no-assembler,-llvm-header-guard,-bugprone-easily-swappable-parameters,-modernize-macro-to-enum,-cert-dcl51-cpp,-cert-dcl37-c,-bugprone-reserved-identifier,-readability-identifier-length,-altera-unroll-loops --warnings-as-errors=* -header-filter='.*' -- $(CFLAGS) 

dis: dissasemble
dissasemble: build/AchieveBIOS.bin
	riscv64-unknown-elf-objdump -d -S -l --visualize-jumps -M numeric,no-aliases --disassembler-color=color --inlines build/main.o
