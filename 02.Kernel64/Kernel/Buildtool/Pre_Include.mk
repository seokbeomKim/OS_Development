NASM64=nasm -f elf64
GCC64=gcc -m64 -c -ffreestanding -I${KERNEL_DIR}
LD64=ld -melf_x86_64 -T elf_x86_64.x -nostdlib -e Main -Ttext 0x200000
OBJCOPY64=objcopy -j .text -j .data -j .rodata -j .bss -S -O binary

C_SOURCES=$(wildcard *.c)
ASM_SOURCES=$(wildcard *.asm)

ENTRY_OBJ_FILES=EntryPoint.o
C_OBJ_FILES=$(notdir $(patsubst %.c,%.o,$(C_SOURCES)))
ASM_OBJ_FILES=$(notdir $(patsubst %.asm,%.o,$(ASM_SOURCES)))

MAKE_OPTION=-s