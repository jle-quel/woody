#include <woody.h>

typedef struct
{
	int ac;
	void (*f)(char **av);
} t_state;

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

static inline void init_random_woody(char **av)
{
	char *key = NULL;

	key = generate_key(DEFAULT_SIZE);
	woody(av[1], key);
	free(key);
}

static inline void init_argv_woody(char **av)
{
	woody(av[1], av[2]);
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

static const t_state state[] =
{
	(const t_state){MIN_ARGV_SIZE, &init_random_woody},
	(const t_state){MAX_ARGV_SIZE, &init_argv_woody},
};

int main(int ac, char **av)
{
	const uint8_t limit = 2;

	for (uint8_t index = 0; index < limit; index++)
	{
		if (ac == state[index].ac)
		{
			state[index].f(av);
			exit(0);
		}
	}

	error(USAGE, NULL);
}
