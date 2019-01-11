#include <woody.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

static inline void usage(char const *filename)
{
	(void)filename;
	fprintf(stderr, "usage: woody_woodpacker [elf x86_64] [key]\n");
}

static inline void wrong_format(char const *filename)
{
	fprintf(stderr, "\"%s\" is not an elf file format\n", filename);
}

static inline void wrong_architecture(char const *filename)
{
	fprintf(stderr, "\"%s\" is not an x86_64 binary\n", filename);
}

static inline void malloc_fail(char const *filename)
{
	(void)filename;
	fprintf(stderr, "malloc failed to allocate memory\n");
}

static inline void wrong_fd(char const *filename)
{
	fprintf(stderr, "error in opening file \"%s\"\n", filename);
}

static inline void stat_fail(char const *filename)
{
	fprintf(stderr, "error in getting information on file \"%s\"\n", filename);
}

static inline void mmap_fail(char const *filename)
{
	(void)filename;
	fprintf(stderr, "mmap failed to allocate memory\n");
}

static inline void corruption(char const *filename)
{
	fprintf(stderr, "file \"%s\" is corrupted or truncated\n", filename);
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

const t_vector vector[] =
{
	(const t_vector){USAGE, &usage},
	(const t_vector){WRONG_FORMAT, &wrong_format},
	(const t_vector){WRONG_ARCHITECTURE, &wrong_architecture},
	(const t_vector){MALLOC_FAIL, &malloc_fail},
	(const t_vector){WRONG_FD, &wrong_fd},
	(const t_vector){STAT_FAIL, &stat_fail},
	(const t_vector){MMAP_FAIL, &mmap_fail},
	(const t_vector){CORRUPTION, &corruption},
};

void error(t_error const err, char const *filename)
{
	static const uint8_t limit = 8;

	for (uint8_t index = 0; index < limit; index++)
	{
		if (err == vector[index].err)
		{
			vector[index].f(filename);
			exit(vector[index].err);
		}
	}
}
