NASM64=nasm -f elf64
GCC64=gcc -m64 -c -ffreestanding
LD64=ld -melf_x86_64 -T elf_x86_64.x -nostdlib -e Main -Ttext 0x200000
OBJCOPY64=objcopy -j .text -j .data -j .rodata -j .bss -S -O binary

OBJECT_DIRECTORY = Temp
SOURCE_DIRECTORY = Source

ROOT_DIRECTORY = $(PWD)
C_SOURCE_FILES = $(wildcard Source/*.c)

all: prepare Kernel64.bin

prepare:
	@mkdir -p $(OBJECT_DIRECTORY)
	
clean:
	@echo ">>> Clean the kernel image..."
	@rm -f *.bin
	@rm -rf $(OBJECT_DIRECTORY)
	@rm -f *.o
	@rm -f *.elf
	@make -C Kernel clean
	
dep:
	@echo ">>> Generate dependency file..."
	@gcc -MM $(C_SOURCE_FILES) > Dependency.dep

Kernel64.bin:
	@echo ">>> Comple & Generate a kernel image..."
	@make -C Kernel -s
	@mv Kernel/Kernel64.bin $@
	#mv Kernel/EntryPoint64.bin $(OBJECT_DIRECTORY)/EntryPoint64.bin