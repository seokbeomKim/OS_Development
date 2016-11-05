all: Utility BootLoader Kernel32 Kernel64 Disk.img

BootLoader:
	@echo "* Prepare a boot loader..."

	@make -C 00.BootLoader -s 
	
Kernel32:
	@echo "* Build a 32bit kernel..."
	
	@make -C 01.Kernel32 -s

Kernel64:
	@echo "* Build 64bit kernel..."
	
	@make -C 02.Kernel64 -s

Disk.img: 00.BootLoader/BootLoader.bin 01.Kernel32/Kernel32.bin 02.Kernel64/Kernel64.bin
	@echo "* Generate disk image with ImageMaker...";
	@./ImageMaker  00.BootLoader/BootLoader.bin 01.Kernel32/Kernel32.bin 02.Kernel64/Kernel64.bin

clean:
	@echo "* Cleaning..."
	@make -C 00.BootLoader clean
	@make -C 01.Kernel32 clean
	@make -C 02.Kernel64 clean
	@make -C 04.Utility clean
	@rm -f Disk.img
	@rm -f ImageMaker

Utility:
	@echo "* Build utilities..."
	@make -C 04.Utility -s
	
	@cp 04.Utility/00.ImageMaker/ImageMaker ./ImageMaker