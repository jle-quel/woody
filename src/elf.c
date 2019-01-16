#include <woody.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

static inline void map_error(t_error const err, char const *filename, int fd)
{
	close(fd);
	error(err, filename);
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

t_elf *get_elf(char const *filename)
{
	t_elf *elf = NULL;
	int fd = 0;
	struct stat buf;

	elf = (t_elf *)constructor(sizeof(t_elf));
	
	if ((fd = open(filename, O_RDONLY)) == -1)
		error(WRONG_FD, filename);
	if (stat(filename, &buf) == -1)
		map_error(STAT_FAIL, filename, fd);
	if ((elf->ptr = mmap(NULL, buf.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
		map_error(MMAP_FAIL, filename, fd);

	close(fd);

	elf->filesize = buf.st_size;
	elf->filename = filename;

	return elf;
}

void release_elf(t_elf *elf)
{
	munmap(elf->ptr, elf->filesize);
	free((void *)elf);
}
