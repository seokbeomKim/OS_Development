all: Utility BootLoader Kernel32 Kernel64 Disk.img

BootLoader:
	@echo "Build Boot Loader"

	make -C 00.BootLoader
	
Kernel32:
	@echo "Build 32bit Kernel"
	
	make -C 01.Kernel32

Kernel64:
	@echo "Build 64bit Kernel"
	
	make -C 02.Kernel64

Disk.img: 00.BootLoader/BootLoader.bin 01.Kernel32/Kernel32.bin 02.Kernel64/Kernel64.bin
	@echo "Build Disk Image"
	./ImageMaker  00.BootLoader/BootLoader.bin 01.Kernel32/Kernel32.bin 02.Kernel64/Kernel64.bin

clean:
	make -C 00.BootLoader clean
	make -C 01.Kernel32 clean
	make -C 02.Kernel64 clean
	make -C 04.Utility clean
	rm -f Disk.img
	rm -f ImageMaker

Utility:
	make -C 04.Utility
	
	cp 04.Utility/00.ImageMaker/ImageMaker ./ImageMaker