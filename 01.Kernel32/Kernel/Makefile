NASM32=nasm -f elf32
GCC32=gcc -m32 -c -ffreestanding
LD32=ld -melf_i386 -T elf_i386.x -nostdlib -e Main -Ttext 0x10200
OBJCOPY32=objcopy -j .text -j .data -j .rodata -j .bss -S -O binary

C_SOURCES=$(wildcard *.c)
ASM_SOURCES=$(wildcard *.asm)

C_OBJ_FILES=$(notdir $(patsubst %.c,%.o,$(C_SOURCES)))
ASM_OBJ_FILES=$(notdir $(patsubst %.asm,%.o,$(ASM_SOURCES)))

all: dep Kernel32.elf.bin
	@echo ">>> Kernel32.elf.bin has been created."
	
Kernel32.elf.bin: Kernel32.elf
	@$(OBJCOPY32) Kernel32.elf $@
	
%.o: %.c
	@$(GCC32) -c $<

%.o: %.asm
	@$(NASM32) -o $@ $<

Kernel32.elf: $(C_OBJ_FILES) $(ASM_OBJ_FILES)
	@$(LD32) -o $@ $^

clean:
	@rm -rf *.bin *.elf *.o
	
dep:
	@$(GCC32) -MM $(C_SOURCES) > Dependency.dep
	
ifeq (Dependency.dep, $(wildcard Dependency.dep))
include Dependency.dep
endif