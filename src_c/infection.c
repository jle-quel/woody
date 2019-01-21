#include <woody.h>

uint8_t payload[] =
{
  0x50, 0x57, 0x56, 0x52, 0x51, 0x53, 0xe8, 0x0c, 0x00, 0x00,
  0x00, 0x2e, 0x2e, 0x2e, 0x57, 0x4f, 0x4f, 0x44, 0x59, 0x2e,
  0x2e, 0x2e, 0x0a, 0xbf, 0x01, 0x00, 0x00, 0x00, 0x5e, 0xba,
  0x0c, 0x00, 0x00, 0x00, 0xb8, 0x01, 0x00, 0x00, 0x00, 0x0f,
  0x05, 0x48, 0x31, 0xd2, 0x48, 0x31, 0xc9, 0x48, 0x8d, 0x3d,
  0xca, 0xff, 0xff, 0xff, 0xbe, 0x42, 0x00, 0x00, 0x00, 0x48,
  0xba, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x52,
  0x48, 0xba, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38,
  0x52, 0x39, 0xf1, 0x74, 0x27, 0x48, 0x31, 0xc0, 0x48, 0x31,
  0xd2, 0x48, 0x31, 0xdb, 0x48, 0x89, 0xc8, 0xbb, 0x10, 0x00,
  0x00, 0x00, 0x48, 0xf7, 0xfb, 0x48, 0x89, 0xd3, 0x48, 0x31,
  0xc0, 0x8a, 0x04, 0x0f, 0x32, 0x04, 0x1c, 0x88, 0x04, 0x0f,
  0xff, 0xc1, 0xeb, 0xd5, 0x58, 0x58, 0x5b, 0x59, 0x5a, 0x5e,
  0x5f, 0x58, 0xe9, 0xba, 0xba, 0xfe, 0xca
};

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

static inline void set_payload(t_elf const *elf, char *key)
{
	Elf64_Addr const entry_point = elf->old_entrypoint - (elf->segment_addr + elf->segment_size) - PAYLOAD_SIZE;
	Elf64_Off const entry_encryption = entry_point + (PAYLOAD_SIZE - 50 - 4);
	Elf64_Off const limit = elf->section_size - (elf->old_entrypoint - elf->section_addr);
	size_t const size = sizeof(int);

	_memcpy(&payload[50], &entry_encryption, size);
	_memcpy(&payload[55], &limit, size);
	_memcpy(&payload[61], key + 8, 8);
	_memcpy(&payload[72], key, 8);
	_memcpy(&payload[PAYLOAD_SIZE - size], &entry_point, size);
}

static void write_on_memory(t_elf const *elf, char *ptr, char const *key)
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
		*dst++ = *src++;
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

	_memcpy(ptr + beg_payload, payload, PAYLOAD_SIZE);
	_xor(ptr + beg_encrypt, end_encrypt - beg_encrypt, key);
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void create_infected(t_elf const *elf)
{
	int fd = 0;
	char *ptr = NULL;
	char *key = NULL;
	char const *filename = "woody";

	if ((fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0700)) == -1)
		error(WRONG_FD, filename); 
	if ((ptr = mmap(NULL, elf->filesize + PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0)) == MAP_FAILED)
		error(MMAP_FAIL, filename);

	printf("Encryption key: %s\n", (key = generate_key(DEFAULT_SIZE)));

	set_payload(elf, key);
	write_on_memory(elf, ptr, key);
	write(fd, ptr, elf->filesize + PAGE_SIZE);

	munmap(ptr, elf->filesize + PAGE_SIZE);
	close(fd);
	free(key);
}
