#include <woody.h>

typedef struct
{
	t_error const err;
	void (*f)(char const *str);
} t_state;

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

static inline void usage(VOID char const *filename)
{
	fprintf(stderr, "usage: woody_woodpacker [elf x86_64]\n");
}

static inline void wrong_format(char const *filename)
{
	fprintf(stderr, "\"%s\" is not an elf file format\n", filename);
}

static inline void wrong_architecture(char const *filename)
{
	fprintf(stderr, "\"%s\" is not an x86_64 binary\n", filename);
}

static inline void not_executable(char const *filename)
{
	fprintf(stderr, "\"%s\" is not executable\n", filename);
}

static inline void malloc_fail(VOID char const *filename)
{
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

static inline void mmap_fail(VOID char const *filename)
{
	fprintf(stderr, "mmap failed to allocate memory\n");
}

static inline void corruption(char const *filename)
{
	fprintf(stderr, "file \"%s\" is corrupted or truncated\n", filename);
}

static inline void packed(char const *filename)
{
	fprintf(stderr, "file \"%s\" is already packed\n", filename);
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

static const t_state state[] =
{
	(const t_state){USAGE, &usage},
	(const t_state){WRONG_FORMAT, &wrong_format},
	(const t_state){WRONG_ARCHITECTURE, &wrong_architecture},
	(const t_state){NOT_EXEC, &not_executable},
	(const t_state){MALLOC_FAIL, &malloc_fail},
	(const t_state){WRONG_FD, &wrong_fd},
	(const t_state){STAT_FAIL, &stat_fail},
	(const t_state){MMAP_FAIL, &mmap_fail},
	(const t_state){CORRUPTION, &corruption},
	(const t_state){PACKED, &packed},
};

void error(t_error const err, char const *filename)
{
	static const uint8_t limit = 10;

	for (uint8_t index = 0; index < limit; index++)
	{
		if (err == state[index].err)
		{
			state[index].f(filename);
			exit(state[index].err);
		}
	}
}
