include ${KERNEL_DIR}/Buildtool/Pre_Include.mk

all: $(ASM_OBJ_FILES)
	@echo ">>> Finish compiling assembly files...";
	@mv *.o ${KERNEL_DIR}/Temp_ASM/

%.o: %.asm
	$(NASM64) -o $@ $<
	
clean: 
	@rm *.o