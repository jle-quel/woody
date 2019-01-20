#include <woody.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

static inline bool is_elf(t_elf const *elf)
{
	if (elf->ptr + ELF_MAGIC_SIZE >= elf->ptr + elf->filesize)
		error(CORRUPTION, elf->filename);

	return *(uint32_t *)elf->ptr == ELF_MAGIC_NUMBER;
}

static inline bool is_x86(t_elf const *elf)
{
	Elf64_Ehdr const *header = (Elf64_Ehdr const *)elf->ptr;

	if (elf->ptr + sizeof(Elf64_Ehdr) >= elf->ptr + elf->filesize)
		error(CORRUPTION, elf->filename);

	return header->e_ident[EI_CLASS] == X86_64;
}

static inline bool is_packed(t_elf const *elf)
{
	Elf64_Ehdr const *header = (Elf64_Ehdr const *)elf->ptr;

	if (elf->ptr + sizeof(Elf64_Ehdr) >= elf->ptr + elf->filesize)
		error(CORRUPTION, elf->filename);

	return *(uint32_t *)((char *)&header->e_ident[EI_PAD]) == PACK_MAGIC_NUMBER;
}

static inline bool is_executable(t_elf const *elf)
{
	Elf64_Ehdr const *header = (Elf64_Ehdr const *)elf->ptr;

	if (elf->ptr + sizeof(Elf64_Ehdr) >= elf->ptr + elf->filesize)
		error(CORRUPTION, elf->filename);

	return header->e_entry != 0;
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void woody(char const *filename)
{
	t_elf *elf = NULL;

	elf = get_elf(filename);

	if (is_elf(elf) == false)
		error(WRONG_FORMAT, elf->filename);
	if (is_x86(elf) == false)
		error(WRONG_ARCHITECTURE, elf->filename);
	if (is_packed(elf) == true)
		error(PACKED, elf->filename);
	if (is_executable(elf) == false)
		error(NOT_EXEC, elf->filename);

	modify_segments(elf);
	modify_sections(elf);
	modify_header(elf);

	create_infected(elf);

	release_elf(elf);
}

