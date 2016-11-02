NASM64=nasm -f elf64
GCC64=gcc -m64 -c -ffreestanding
LD64=ld -melf_x86_64 -T elf_x86_64.x -nostdlib -e Main -Ttext 0x200000
OBJCOPY64=objcopy -j .text -j .data -j .rodata -j .bss -S -O binary

OBJECT_DIRECTORY = Temp
SOURCE_DIRECTORY = Source

ROOT_DIRECTORY = $(PWD)

all: prepare Kernel64.bin

C_SOURCE_FILES = $(wildcard Source/*.c)

prepare:
	mkdir -p $(OBJECT_DIRECTORY)
	
Kernel64.bin: $(OBJECT_DIRECTORY)/Kernel64.elf.bin
	@echo "Generate kernel binary file..."
	cat $(OBJECT_DIRECTORY)/Kernel64.elf.bin $(OBJECT_DIRECTORY)/EntryPoint64.bin > $@
	
clean:
	rm -f *.bin
	rm -rf $(OBJECT_DIRECTORY)
	rm -f *.o
	rm -f *.elf
	make -C Kernel clean
	
dep:
	@echo "Generate dependency file..."
	gcc -MM $(C_SOURCE_FILES) > Dependency.dep

$(OBJECT_DIRECTORY)/Kernel64.elf.bin:
	@echo "Compile Kernel file..."
	make -C Kernel
	mv Kernel/Kernel64.elf.bin $@
	mv Kernel/EntryPoint64.bin $(OBJECT_DIRECTORY)/EntryPoint64.bin