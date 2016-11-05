NASM32=nasm

all: EntryPoint.bin
	@echo ">>> Create entry point binary file..."
	
EntryPoint.bin: EntryPoint.s
	$(NASM32) -o $@ $<

clean:
	rm -f EntryPoint.bin