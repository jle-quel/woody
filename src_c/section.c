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

static inline bool is_entrypoint_section(Elf64_Ehdr const *header, Elf64_Shdr const *section)
{
	return header->e_entry >= section->sh_addr && header->e_entry < section->sh_addr + section->sh_size;
}

static inline bool is_last_section(Elf64_Shdr const *section, t_elf const *elf)
{
	return section->sh_offset + section->sh_size == elf->segment_offset + elf->segment_size;
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
			section->sh_offset += PAGE_SIZE;
		
		if (is_entrypoint_section(header, section) == true)
		{
			elf->section_offset = section->sh_offset;
			elf->section_addr = section->sh_addr;
			elf->section_size = section->sh_size;
		}

		if (is_last_section(section, elf) == true)
		{
			section->sh_size += PAYLOAD_SIZE;

			corrupt = true; 
		}
	}
}

