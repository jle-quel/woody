#include <woody.h>

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

bool is_elf(t_elf const *elf)
{
	if (elf->ptr + ELF_MAGIC_SIZE >= elf->ptr + elf->filesize)
		error(CORRUPTION, elf->filename);

	if (*(uint32_t *)elf->ptr != ELF_MAGIC_NUMBER)
		return false;

	return true;
}

bool is_x86(t_elf const *elf)
{
	Elf64_Ehdr const *header = (Elf64_Ehdr const *)elf->ptr;

	if (elf->ptr + sizeof(Elf64_Ehdr) >= elf->ptr + elf->filesize)
		error(CORRUPTION, elf->filename);

	if (header->e_ident[EI_CLASS] != X86_64)
		return false;

	return true;
}

void *constructor(size_t const size)
{
	void	*ret = NULL;

	if ((ret = malloc(size)) == NULL)
		error(MALLOC_FAIL, NULL);

	return ret;
}
