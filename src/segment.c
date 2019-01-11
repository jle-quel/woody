#include <woody.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

static inline Elf64_Phdr *get_segment(t_elf const *elf, Elf64_Ehdr const *header, uint32_t const index)
{
	Elf64_Phdr *segment = NULL;
	
	segment = (Elf64_Phdr *)((void *)elf->ptr + header->e_phoff + (sizeof(Elf64_Phdr) * index));

	if ((void *)segment >= elf->ptr + elf->filesize)
		error(CORRUPTION, elf->filename);

	return segment;
}

static inline bool is_text(Elf64_Phdr const *segment)
{
	if (segment->p_type != PT_LOAD)
		return false;
	if ((segment->p_flags & PF_X) == false)
		return false;

	return true;
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void modify_segments(t_elf *elf)
{
	Elf64_Ehdr const *header = (Elf64_Ehdr const *)elf->ptr;
	Elf64_Phdr *segment = NULL;
	bool corrupt = 0;

	for (uint32_t index = 0; index < header->e_phnum; index++)
	{
		segment = get_segment(elf, header, index);

		if (corrupt == true)
			segment->p_offset += PAGE_SIZE;

		if (is_text(segment) == true)
		{
			elf->insertion = segment->p_offset + segment->p_filesz;
			elf->new_entry = segment->p_vaddr + segment->p_filesz;

			segment->p_filesz += PAYLOAD_SIZE;
			segment->p_memsz += PAYLOAD_SIZE;
			corrupt = true;
		}
	}
}