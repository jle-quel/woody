#include <woody.h>

uint8_t payload[] =
{
	0x57, 0x56, 0x52, 0xe8, 0x0c, 0x00, 0x00, 0x00, 0x2e, 0x2e,
	0x2e, 0x57, 0x4f, 0x4f, 0x44, 0x59, 0x2e, 0x2e, 0x2e, 0x0a,
	0xbf, 0x01, 0x00, 0x00, 0x00, 0x5e, 0xba, 0x0c, 0x00, 0x00,
	0x00, 0xb8, 0x01, 0x00, 0x00, 0x00, 0x0f, 0x05, 0x5a, 0x5e,
	0x5f, 0xe9, 0xfc, 0xff, 0xff, 0xff
};

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

static inline void set_payload(t_elf const *elf)
{
	int entry = elf->old_entry - elf->new_entry - PAYLOAD_SIZE;

	memcpy(&payload[JMP_INSERTION], &entry, sizeof(int));
}

static void write_on_memory(t_elf const *elf, char *ptr)
{
	memcpy(ptr, elf->ptr, elf->insertion);
	memset(ptr + elf->insertion, 0, PAYLOAD_SIZE);
	memcpy(ptr + elf->insertion + PAGE_SIZE, elf->ptr + elf->insertion, (elf->filesize - elf->insertion) + PAGE_SIZE);

	memcpy(ptr + elf->insertion, payload, PAYLOAD_SIZE);
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void create_infected(t_elf const *elf)
{
	int fd = 0;
	char *ptr = NULL;
	char const *filename = "woody";

	if ((fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0700)) == -1)
		error(WRONG_FD, filename); 
	if ((ptr = (char *)mmap(NULL, elf->filesize + PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0)) == MAP_FAILED)
		error(MMAP_FAIL, filename);

	set_payload(elf);
	write_on_memory(elf, (char *)ptr);
	write(fd, ptr, elf->filesize + PAGE_SIZE);

	munmap(ptr, elf->filesize + PAGE_SIZE);
	close(fd);
}
