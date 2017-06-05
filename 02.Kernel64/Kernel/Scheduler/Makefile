include ${KERNEL_DIR}/Buildtool/Pre_Include.mk

all: $(C_OBJ_FILES)
	@echo ">>> Finish compiling c source files...";
	@mv *.o ${KERNEL_DIR}/Temp_C/

%.o: %.c
	$(GCC64) -o $@ $<
	
clean: 
	@rm *.o