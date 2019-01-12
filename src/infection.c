#include <woody.h>

uint8_t payload[] = {
  0x57, 0x56, 0x52, 0x41, 0x50, 0x41, 0x51, 0xe8, 0x0c, 0x00,
  0x00, 0x00, 0x2e, 0x2e, 0x2e, 0x57, 0x4f, 0x4f, 0x44, 0x59,
  0x2e, 0x2e, 0x2e, 0x0a, 0xbf, 0x01, 0x00, 0x00, 0x00, 0x5e,
  0xba, 0x0c, 0x00, 0x00, 0x00, 0xb8, 0x01, 0x00, 0x00, 0x00,
  0x0f, 0x05, 0x41, 0x59, 0x41, 0x58, 0x5a, 0x5e, 0x5f, 0xe9,
  0xba, 0xba, 0xfe, 0xca
};

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

static inline void set_payload(VOID t_elf const *elf)
{
	printf("%zu\n", sizeof(payload));
	int const entry = elf->old_entry - elf->new_entry - PAYLOAD_SIZE;

	_memcpy(&payload[50], &entry, sizeof(int));
}

static void write_on_memory(t_elf const *elf, void *ptr)
{
	char *key = NULL;

	key = get_key(KEY_SIZE);

	printf("encryption key \"%s\"\n", key);

	_memcpy(ptr, elf->ptr, elf->old_offset);
//	_xorcpy(ptr + elf->old_offset, elf->ptr + elf->old_offset, elf->new_offset - elf->old_offset, key);
	_memcpy(ptr + elf->old_offset, elf->ptr + elf->old_offset, elf->new_offset - elf->old_offset);

	_memset(ptr + elf->new_offset, 0, PAGE_SIZE);
	_memcpy(ptr + elf->new_offset + PAGE_SIZE, elf->ptr + elf->new_offset, (elf->filesize - elf->new_offset) + PAGE_SIZE);

	_memcpy(ptr + elf->new_offset, payload, PAYLOAD_SIZE);

	free(key);
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
	if ((ptr = mmap(NULL, elf->filesize + PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0)) == MAP_FAILED)
		error(MMAP_FAIL, filename);

	set_payload(elf);
	write_on_memory(elf, ptr);
	write(fd, ptr, elf->filesize + PAGE_SIZE);

	munmap(ptr, elf->filesize + PAGE_SIZE);
	close(fd);
}
