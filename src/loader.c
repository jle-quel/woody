#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <elf.h>
#include <string.h>

uint8_t shellcode[] =
{
	0x57, 0x56, 0x52, 0xe8, 0x0c, 0x00, 0x00, 0x00, 0x2e, 0x2e,
	0x2e, 0x57, 0x4f, 0x4f, 0x44, 0x59, 0x2e, 0x2e, 0x2e, 0x0a,
	0xbf, 0x01, 0x00, 0x00, 0x00, 0x5e, 0xba, 0x0c, 0x00, 0x00,
	0x00, 0xb8, 0x01, 0x00, 0x00, 0x00, 0x0f, 0x05, 0x5a, 0x5e,
	0x5f, 0xe9, 0xfc, 0xff, 0xff, 0xff
};

int shellcode_size = sizeof(shellcode);

void *ptr;
int fd;
struct stat buf;
Elf64_Ehdr *header;
Elf64_Phdr *segment;
Elf64_Shdr *section;
int corrupt;
int insertion_offset;
int new_entry;
int old_entry;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void modify_payload()
{
	int result = (old_entry - new_entry) - 46;
	printf("jmp offset: %x\n", result);

	memcpy(&shellcode[42], &result, sizeof(int));
}

void modify_segments(void)
{
	corrupt = 0;

	for (uint32_t index = 0; index < header->e_phnum; index++)
	{
		segment = ptr + header->e_phoff + (sizeof(Elf64_Phdr) * index);

		if (corrupt)
			segment->p_offset += 4096;

		if (segment->p_type == PT_LOAD && (segment->p_flags & PF_X))
		{
			insertion_offset = segment->p_offset + segment->p_filesz;
			new_entry = segment->p_vaddr + segment->p_filesz;
			printf("insertion: %x\n", insertion_offset);
			printf("entry:     %x\n", new_entry);

			segment->p_filesz += shellcode_size;
			segment->p_memsz += shellcode_size;
			corrupt |= 1;
		}
	}
}

void modify_sections(void)
{
	corrupt = 0;

	for (uint32_t index = 0; index < header->e_shnum; index++)
	{
		section = ptr + header->e_shoff + (sizeof(Elf64_Shdr) * index);

		if (corrupt)
			section->sh_offset += 4096;

		if (section->sh_addr + section->sh_size == insertion_offset)
		{
			section->sh_size += shellcode_size;
			corrupt |= 1;
		}
	}
}

void modify_header(void)
{
	header->e_shoff += 4096;
	old_entry = header->e_entry;
	header->e_entry = new_entry;
}

void create_file(void)
{
	int new_file = open("file", O_RDWR | O_CREAT | O_TRUNC, 0700);
	void *new_ptr = mmap(NULL, buf.st_size + 4096, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	int index = 0;
	char *tmp = new_ptr;
	char *tmp2 = ptr;

	while (index < insertion_offset)
	{
		tmp[index] = *tmp2;
		tmp2++;
		index++;
	}
	printf("%x\n", index);
	
	for (int i = 0; i < shellcode_size; i++)
	{
		tmp[index] = shellcode[i];
		index++;
	}
	for (int i = 0; i < 4096 - shellcode_size; i++)
	{
		tmp[index] = 0;
		index++;
	}
	while (index < buf.st_size + 4096)
	{
		tmp[index] = *tmp2;
		tmp2++;
		index++;
	}

	write(new_file, tmp, buf.st_size + 4096);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int ac, char **av)
{
	fd = open(av[1], O_RDONLY);
	stat(av[1], &buf);
	ptr = mmap(0, buf.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
	header = ptr;

	modify_segments();
	modify_sections();
	modify_header();

	modify_payload();

	create_file();
}
