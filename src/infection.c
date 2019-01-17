#include <woody.h>

uint8_t payload[] =
{
  0x50, 0x57, 0x56, 0x52, 0x41, 0x50, 0xe8, 0x0c, 0x00, 0x00,
  0x00, 0x2e, 0x2e, 0x2e, 0x57, 0x4f, 0x4f, 0x44, 0x59, 0x2e,
  0x2e, 0x2e, 0x0a, 0xbf, 0x01, 0x00, 0x00, 0x00, 0x5e, 0xba,
  0x0c, 0x00, 0x00, 0x00, 0xb8, 0x01, 0x00, 0x00, 0x00, 0x0f,
  0x05, 0x4d, 0x31, 0xc0, 0x48, 0x31, 0xc0, 0xb8, 0x42, 0x00,
  0x00, 0x00, 0x4c, 0x8d, 0x05, 0x00, 0x00, 0x00, 0x00, 0x83,
  0xf8, 0x00, 0x74, 0x13, 0x48, 0x31, 0xd2, 0x41, 0x8a, 0x10,
  0x80, 0xf2, 0x2a, 0x41, 0x88, 0x10, 0xff, 0xc8, 0x49, 0xff,
  0xc0, 0xeb, 0xe8, 0x41, 0x58, 0x5a, 0x5e, 0x5f, 0x58, 0xe9,
  0xba, 0xba, 0xfe, 0xca
};

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

static inline void set_payload(t_elf const *elf)
{
	Elf64_Addr const entry_point = elf->old_entrypoint - (elf->segment_addr + elf->segment_size) - PAYLOAD_SIZE;
	Elf64_Off const limit = elf->section_size - (elf->old_entrypoint - elf->section_addr);
	Elf64_Off const entry_encryption = entry_point + (PAYLOAD_SIZE - 55 - 4);
	size_t const size = sizeof(int);

	_memcpy(&payload[48], &limit, size);
	_memcpy(&payload[55], &entry_encryption, size);
	_memcpy(&payload[PAYLOAD_SIZE - size], &entry_point, size);
}

static void write_on_memory(t_elf const *elf, char *ptr)
{
	Elf64_Off const beg_encrypt = elf->section_offset + (elf->old_entrypoint - elf->section_addr);
	Elf64_Off const end_encrypt = elf->section_offset + elf->section_size;
	Elf64_Off const beg_payload = elf->segment_offset + elf->segment_size;
	Elf64_Off const end_payload = elf->segment_offset + elf->segment_size + PAGE_SIZE;
	Elf64_Off const end_file = elf->filesize + PAGE_SIZE;

	size_t index = 0;
	char *dst = ptr;
	char *src = elf->ptr;

	while (index < beg_encrypt)
	{
		*dst++ = *src++;
		index++;
	}
	while (index < end_encrypt)
	{
		*dst++ = *src ^ 42;
		src++;
		index++;
	}

	while (index < beg_payload)
	{
		*dst++ = *src++;
		index++;
	}
	while (index < end_payload)
	{
		*dst++ = 0;
		index++;
	}

	while (index < end_file)
	{
		*dst++ = *src++;
		index++;
	}

	_memcpy(ptr + beg_payload, payload, sizeof(payload));
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void create_infected(t_elf const *elf, VOID char const *key)
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
