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

static inline void modify_segment(Elf64_Phdr *segment)
{
	segment->p_offset += PAGE_SIZE;
}

static inline bool is_text_segment(Elf64_Phdr const *segment)
{
	if (segment->p_type != PT_LOAD)
		return false;
	if ((segment->p_flags & PF_X) == false)
		return false;

	return true;
}

static inline void modify_elf(t_elf *elf, Elf64_Phdr const *segment)
{
	elf->old_offset = segment->p_offset;
	elf->new_offset = segment->p_offset + segment->p_filesz;

	elf->new_entry = segment->p_vaddr + segment->p_filesz;
}

static inline void modify_text_segment(Elf64_Phdr *segment)
{
	segment->p_filesz += PAYLOAD_SIZE;
	segment->p_memsz += PAYLOAD_SIZE;
	segment->p_flags |= PF_W;
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
			modify_segment(segment);

		if (is_text_segment(segment) == true)
		{
			modify_elf(elf, segment);
			modify_text_segment(segment);

			corrupt = true;
		}
	}
}
