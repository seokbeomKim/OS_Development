NASM64=nasm -f elf64
GCC64=gcc -m64 -c -ffreestanding
LD64=ld -melf_x86_64 -T elf_x86_64.x -nostdlib -e Main -Ttext 0x200000
OBJCOPY64=objcopy -j .text -j .data -j .rodata -j .bss -S -O binary

C_SOURCES=$(wildcard *.c)
ASM_SOURCES=$(wildcard *.asm)

C_OBJ_FILES=$(notdir $(patsubst %.c,%.o,$(C_SOURCES)))
ASM_OBJ_FILES=$(notdir $(patsubst %.asm,%.o,$(ASM_SOURCES)))

all: dep EntryPoint64 Kernel64.elf.bin
	echo "Kernel64.elf.bin has been created."

EntryPoint64: EntryPoint.s
	$(NASM64) $< -o EntryPoint64.bin

Kernel64.elf.bin: Kernel64.elf
	$(OBJCOPY64) Kernel64.elf $@
	
%.o: %.c
	$(GCC64) -c $<

%.o: %.asm
	$(NASM64) -o $@ $<

Kernel64.elf: $(C_OBJ_FILES) $(ASM_OBJ_FILES)
	$(LD64) -o $@ $^

clean:
	rm -rf *.bin *.elf *.o
	
dep:
	$(GCC64) -MM $(C_SOURCES) > Dependency.dep
	
ifeq (Dependency.dep, $(wildcard Dependency.dep))
include Dependency.dep
endif