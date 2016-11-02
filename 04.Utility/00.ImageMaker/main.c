#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#define BUFSIZE 512
#define SECTOR_SIZE 512
// BUFSIZE는 SECTOR 크기와 같다

int CopyFile(int dst, int src)
{
	int r, w;
	int bytes;

	char buf[BUFSIZE];

	bytes = 0;
	while(1)
	{
		r = read(src, buf, BUFSIZE);
		w = write(dst, buf, r);

		if (r != w) {
			fprintf(stderr, "Read size does not match write size.\n");
			exit(3);
		}

		bytes += r;

		if (r != BUFSIZE) {
			break;
		}
	}

//	printf("bytes = %d\n", bytes);
	return bytes;
}

void ChangeSectorValue(int fd, int total_sectors, int protected_sectors) {
	long pos;

	pos = lseek(fd, 5, SEEK_SET);

	unsigned short data = (unsigned short) total_sectors;
	write (fd, &data, 2);

	data = (unsigned short) protected_sectors;
	write (fd, &data, 2);

	fprintf(stdout, "Total sectors: %d\n", total_sectors);
	fprintf(stdout, "32bit Kernel sectors: %d\n", protected_sectors);
}

int AlignSector(int fd, int size)
{
	int i;
	int count = size % SECTOR_SIZE;
	char ch = 0x00;
	int sectors;

	if (count != 0) {
		count = SECTOR_SIZE - count;

		for (i = 0; i < count; i++) {
			write(fd, &ch, 1);
		}
	}
	sectors = (size + count) / SECTOR_SIZE;
//	printf("sectors = %d\n" + sectors);
	return sectors;
}

int main(int argc, char* argv[])
{
	int fd;	// disk.img 파일
	int src_fd;

	int sector_size_32, sector_size_64;

	if (argc < 4) {
		fprintf(stderr, "[Usage example] %s BootLoader.bin Kernel32.bin Kernel64.bin\n", argv[0]);
		exit(1);
	}

	// Disk 파일 생성
	if ( (fd = open("Disk.img", O_RDWR | O_CREAT | O_TRUNC, 0755)) == -1) {
		fprintf(stderr, "Failed to create Disk.img file.\n");
		exit(2);
	}

	// Bootloader
	if ( (src_fd = open(argv[1], O_RDONLY)) == -1) {
		fprintf(stderr, "Failed to open %s.\n", argv[1]);
		exit(2);
	}

	CopyFile(fd, src_fd);
	close(src_fd);

	AlignSector(fd, sector_size_32);

	// Kernel32
	if ( (src_fd = open(argv[2], O_RDONLY)) == -1) {
		fprintf(stderr, "Failed to open %s.\n", argv[2]);
		exit(2);
	}

	sector_size_32 = CopyFile(fd, src_fd);
	close(src_fd);

	sector_size_32 = AlignSector(fd, sector_size_32);

	// Kernel64
	if ( (src_fd = open(argv[3], O_RDONLY)) == -1) {
		fprintf(stderr, "Failed to open %s.\n", argv[3]);
		exit(2);
	}

	sector_size_64 = CopyFile(fd, src_fd);
	close(src_fd);

	sector_size_64 = AlignSector(fd, sector_size_64);

	ChangeSectorValue(fd, sector_size_32 + sector_size_64, sector_size_32);

	return 0;
}

