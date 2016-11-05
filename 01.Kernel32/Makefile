NASM32 = nasm
GCC32 = gcc -m32 -c -ffreestanding
LD32 = ld -melf_i386 -T elf_i386.x -nostdlib -e Main -Ttext 0x10200
OBJCOPY32 = objcopy -j .text -j .data -j .rodata -j .bss -S -O binary

OBJECT_DIRECTORY = Temp
SOURCE_DIRECTORY = Source

ROOT_DIRECTORY = $(PWD)

all: prepare Kernel32.bin

C_SOURCE_FILES = $(wildcard Source/*.c)

prepare:
	@mkdir -p $(OBJECT_DIRECTORY)
	

Kernel32.bin: $(OBJECT_DIRECTORY)/EntryPoint.bin \
			$(OBJECT_DIRECTORY)/Kernel32.elf.bin
	@echo ">>> Generate 32bits kernel binary..."
	@cat $^ > $@
	
clean:
	@rm -f *.bin
	@rm -rf $(OBJECT_DIRECTORY)
	@rm -f *.o
	@rm -f *.elf
	@make -C Kernel clean
	@make -C EntryPoint clean
	
dep:
	@echo ">>> Generate dependency file..."
	@gcc -MM $(C_SOURCE_FILES) > Dependency.dep
	
$(OBJECT_DIRECTORY)/EntryPoint.bin: 
	@echo ">>> Build an entry point..."
	@make -C EntryPoint
	@mv EntryPoint/EntryPoint.bin $@

$(OBJECT_DIRECTORY)/Kernel32.elf.bin:
	@echo ">>> Build a kernel file..."
	@make -C Kernel
	@mv Kernel/Kernel32.elf.bin $@