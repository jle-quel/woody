#include <woody.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

static inline void map_error(t_error const err, char const *filename, int fd)
{
	close(fd);
	error(err, filename);
}


static void map_file(t_elf *elf)
{
	int fd = 0;
	struct stat buf;

	if ((fd = open(elf->filename, O_RDONLY)) == -1)
		error(WRONG_FD, elf->filename);
	if (stat(elf->filename, &buf) == -1)
		map_error(STAT_FAIL, elf->filename, fd);
	if ((elf->ptr = mmap(NULL, buf.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
		map_error(MMAP_FAIL, elf->filename, fd);

	elf->filesize = buf.st_size;

	close(fd);
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

t_elf *get_elf(char const *filename)
{
	t_elf *elf = NULL;

	elf = (t_elf *)constructor(sizeof(t_elf));

	elf->filename = filename;

	map_file(elf);

	return elf;
}

void release_elf(t_elf *elf)
{
	munmap(elf->ptr, elf->filesize);
	free((void *)elf);
}
