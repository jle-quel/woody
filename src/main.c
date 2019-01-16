#include <woody.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

static void woody(char const *filename)
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

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

int main(int ac, char **av)
{
	if (ac != VALID_ARGV_SIZE)
		error(USAGE, NULL);

	woody(av[1]);

	return 0;
}
