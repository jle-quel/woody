#include <woody.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

bool _is_digit(int c)
{
	if (c >= 49 && c <= 57)
		return true;

	return false;
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

unsigned char *generate_key(const size_t size)
{
	unsigned char *key = NULL;
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
		if (_is_digit(c) == true)
		{
			key[index] = c;
			index++;
		}
	}

	key[index] = 0;

	return key;
}
