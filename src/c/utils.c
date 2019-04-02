#include <woody.h>

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

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

size_t _strlen(const char *str)
{
	size_t index = 0;

	if (str == NULL)
		return 0;

	while (str[index])
		index++;

	return index;
}
