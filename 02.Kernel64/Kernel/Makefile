export KERNEL_DIR=$(shell pwd)

include Buildtool/Pre_Include.mk

all: prepare dep Module Kernel64.bin
	@echo $(C_OBJ_FILES) $(ASM_OBJ_FILES)
	@echo ">>> Kernel64.elf.bin has been created."

Module:
	@echo ">>> Build Asm directory...";
	@make -C Asm ${MAKE_OPTION} KERNEL_DIR=${KERNEL_DIR} 
	
	@echo ">>> Build DataType directory...";
	@make -C DataType ${MAKE_OPTION} KERNEL_DIR=${KERNEL_DIR}
	
	@echo ">>> Build DeviceDriver directory...";
	@make -C DeviceDriver ${MAKE_OPTION} KERNEL_DIR=${KERNEL_DIR}
	
	@echo ">>> Build Include library...";
	@make -C Include ${MAKE_OPTION} KERNEL_DIR=${KERNEL_DIR}
	
	@echo ">>> Build Init directory...";
	@make -C Init ${MAKE_OPTION} KERNEL_DIR=${KERNEL_DIR}

$(ENTRY_OBJ_FILES): EntryPoint.s
	@$(NASM64) $< -o $@

Kernel64.bin: Kernel64.elf
	@$(OBJCOPY64) Kernel64.elf $@
	
%.o: %.c
	@$(GCC64) -c $<

%.o: %.asm
	@$(NASM64) -o $@ $<

Kernel64.elf: $(ENTRY_OBJ_FILES) $(C_OBJ_FILES) $(ASM_OBJ_FILES)
	@mv Main.o Temp_C/
	$(LD64) -o $@ EntryPoint.o Temp_C/*.o Temp_ASM/*.o 

clean:
	@echo ">>> Cleaning kernel64 generated files...";
	@rm -f *.bin *.elf
	@rm -rf Temp_ASM
	@rm -rf Temp_C
	
dep:
	@echo ">>> Generating dependency file...";
	@$(GCC64) -MM $(C_SOURCES) > Dependency.dep
	
prepare:
	@mkdir -p ${KERNEL_DIR}/Temp_ASM ${KERNEL_DIR}/Temp_C
	
ifeq (Dependency.dep, $(wildcard Dependency.dep))
include Dependency.dep
endif