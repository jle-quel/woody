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

static inline bool is_entrypoint_segment(Elf64_Ehdr const *header, Elf64_Phdr const *segment)
{
	if (header->e_entry < segment->p_vaddr)
		return false;
	if (header->e_entry > segment->p_vaddr + segment->p_filesz)
		return false;

	return true;
}

static inline bool is_segment_corrupted(t_elf const *elf)
{
	if (elf->segment_offset >= elf->filesize)
		return true;

	if (elf->segment_size == 0)
		return true;
	
	return false;
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

		if (is_entrypoint_segment(header, segment) == true)
		{
			elf->segment_offset = segment->p_offset;
			elf->segment_addr = segment->p_vaddr;
			elf->segment_size = segment->p_filesz;
			
			segment->p_filesz += PAYLOAD_SIZE;
			segment->p_memsz += PAYLOAD_SIZE;
			segment->p_flags |= PF_W;

			corrupt = true;
		}
	}

	if (segment == NULL || is_segment_corrupted(elf) == true)
		error(CORRUPTION, elf->filename);

}
