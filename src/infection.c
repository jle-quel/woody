#include <woody.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

/*
static inline void set_payload(t_elf const *elf)
{
	printf("%zu\n", sizeof(payload));
	int const entry = elf->old_entry - elf->new_entry - PAYLOAD_SIZE;

	_memcpy(&payload[52], &elf->text_addr, sizeof(int));
	_memcpy(&payload[60], &elf->new_entry, sizeof(int));
	_memcpy(&payload[82], &entry, sizeof(int));
}
*/

static void write_on_memory(t_elf const *elf, char *ptr)
{
	Elf64_Off const beg_encrypt = elf->section_offset;
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

//	set_payload(elf);
	write_on_memory(elf, ptr);
	write(fd, ptr, elf->filesize + PAGE_SIZE);

	munmap(ptr, elf->filesize + PAGE_SIZE);
	close(fd);
}
