#include <woody.h>

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void modify_header(t_elf *elf)
{
	Elf64_Ehdr *header = NULL;
	
	header = (Elf64_Ehdr *)elf->ptr;

	if (elf->ptr + sizeof(Elf64_Ehdr) >= elf->ptr + elf->filesize)
		error(CORRUPTION, elf->filename);

	*(uint32_t *)&header->e_ident[EI_PAD] = PACK_MAGIC_NUMBER;

	header->e_shoff += PAGE_SIZE;
	elf->old_entrypoint = header->e_entry;
	header->e_entry = elf->segment_addr + elf->segment_size;
}
