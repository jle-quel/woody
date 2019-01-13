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

	bzero(ret, size);

	return ret;
}

void _memcpy(void *dst, void const *src, const size_t size)
{
	if (dst == NULL)
		return ;
	if (src == NULL)
		return ;

	char *dst_tmp = dst;
	char const *src_tmp = src;

	for (size_t index = 0; index < size; index++)
		dst_tmp[index] = src_tmp[index];
}

void _memset(void *dst, int const c, const size_t size)
{
	if (dst == NULL)
		return ;

	char *dst_tmp = dst;

	for (size_t index = 0; index < size; index++)
		dst_tmp[index] = c; 

}

void _xorcpy(void *dst, void const *src, const size_t size, char const key)
{
	if (dst == NULL)
		return ;
	if (src == NULL)
		return ;

	char *dst_tmp = dst;
	char const *src_tmp = src;

	for (size_t index = 0; index < size; index++)
		dst_tmp[index] = src_tmp[index] ^ key;
}

bool _is_ascii(int c)
{
	if (c < 21)
		return false;
	if (c > 126)
		return false;

	return true;
}

char *get_key(const size_t size)
{
	char *key = NULL;
	int fd = 0;
	char c = 0;
	size_t index = 0;

	if ((key = malloc(size + 1)) == NULL)
		error(MALLOC_FAIL, NULL);
	if ((fd = open("/dev/random", O_RDONLY)) == -1)
		error(WRONG_FD, NULL);

	while (index < size)
	{
		if (read(fd, &c, 1) == -1)
			error(WRONG_FD, NULL);
		if (_is_ascii(c) == true)
		{
			key[index] = c;
			index++;
		}
	}

	key[index] = 0;

	return key;
}
