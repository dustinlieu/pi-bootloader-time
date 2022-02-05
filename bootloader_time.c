#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <bcm_host.h>

#define ST_BASE_OFFSET 0x3000
#define ST_SIZE 22

#define ST_CLO_OFFSET 0x04
#define ST_CHI_OFFSET 0x08

uint64_t get_soc_uptime() {
	int fd = open("/dev/mem", O_RDONLY);
	void *addr = mmap(NULL, ST_SIZE, PROT_READ, MAP_SHARED, fd, bcm_host_get_peripheral_address() + ST_BASE_OFFSET);
	if (addr == (void *) -1) {
		return 0;
	}

	uint32_t st_clo = *((volatile uint32_t*) (addr + ST_CLO_OFFSET));
	uint32_t st_chi = *((volatile uint32_t*) (addr + ST_CHI_OFFSET));
	uint64_t st_counter = ((uint64_t) st_chi << 32) | st_clo;

	munmap(addr, ST_SIZE);
	close(fd);

	return st_counter;
}

uint64_t get_linux_uptime() {
	int fd = open("/proc/uptime", O_RDONLY);
	
	char buffer[32];
	int size = read(fd, buffer, 32);
	if (size == -1) {
		return 0;
	}

	close(fd);

	return (uint64_t) (atof(buffer) * 1000000);
}

int main(int argc, char **argv) {
	uint64_t soc_uptime = get_soc_uptime();
	if (soc_uptime == 0) {
		printf("unable to read soc uptime\n");
		return -1;
	}

	uint64_t linux_uptime = get_linux_uptime();
	if (linux_uptime == 0) {
		printf("unable to read linux uptime\n");
		return -1;
	}

	printf("SoC uptime: %fs\n", ((double) soc_uptime) / 1000000);
	printf("Linux uptime: %fs\n", ((double) linux_uptime) / 1000000);
	printf("Time spent in bootloader: %fs\n", ((double) (soc_uptime - linux_uptime)) / 1000000);
	
	return 0;
}