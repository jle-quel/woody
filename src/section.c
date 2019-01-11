#include <woody.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

static inline Elf64_Shdr *get_section(t_elf const *elf, Elf64_Ehdr const *header, uint32_t const index)
{
	Elf64_Shdr *section = NULL;
	
	section = (Elf64_Shdr *)((void *)elf->ptr + header->e_shoff + (sizeof(Elf64_Shdr) * index));

	if ((void *)section >= elf->ptr + elf->filesize)
		error(CORRUPTION, elf->filename);

	return section;
}

static inline bool is_text(Elf64_Shdr const *section, uint32_t insertion)
{
	if (section->sh_addr + section->sh_size != insertion)
		return false;

	return true;
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void modify_sections(t_elf const *elf)
{
	Elf64_Ehdr const *header = (Elf64_Ehdr const *)elf->ptr;
	Elf64_Shdr *section = NULL;
	bool corrupt = 0;

	for (uint32_t index = 0; index < header->e_shnum; index++)
	{
		section = get_section(elf, header, index);

		if (corrupt == true)
			section->sh_offset += PAGE_SIZE;

		if (is_text(section, elf->new_offset) == true)
		{
			section->sh_size += PAYLOAD_SIZE;
			corrupt = true; 
		}
	}
}
