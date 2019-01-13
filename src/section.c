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

static inline void modify_section(Elf64_Shdr *section)
{
	section->sh_offset += PAGE_SIZE;
}

static inline bool is_text_section(Elf64_Shdr const *section)
{
	if (section->sh_name == DYN_TEXT)
		return true;
	if (section->sh_name == EXEC_TEXT)
		return true;

	return false;
}

static inline void modify_elf(t_elf *elf, Elf64_Shdr const *section)
{
	elf->text_addr = section->sh_addr;
	elf->text_offset = section->sh_offset;
	elf->text_size = section->sh_size;
}

static inline bool is_last_section(Elf64_Shdr const *section, uint32_t offset)
{
	if (section->sh_addr + section->sh_size != offset)
		return false;

	return true;
}

static inline void modify_last_section(Elf64_Shdr *section)
{
	section->sh_size += PAYLOAD_SIZE;
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void modify_sections(t_elf *elf)
{
	Elf64_Ehdr const *header = (Elf64_Ehdr const *)elf->ptr;
	Elf64_Shdr *section = NULL;
	bool corrupt = 0;

	for (uint32_t index = 0; index < header->e_shnum; index++)
	{
		section = get_section(elf, header, index);

		if (corrupt == true)
			modify_section(section);

		if (is_text_section(section) == true)
			modify_elf(elf, section);

		if (is_last_section(section, elf->new_offset) == true)
		{
			modify_last_section(section);

			corrupt = true; 
		}
	}
}
