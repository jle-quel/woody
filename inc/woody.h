#ifndef WOODY_H
#define WOODY_H

////////////////////////////////////////////////////////////////////////////////
/// INCLUDES 
////////////////////////////////////////////////////////////////////////////////

#include <elf.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>

////////////////////////////////////////////////////////////////////////////////
/// DEFINES 
////////////////////////////////////////////////////////////////////////////////

#define VALID_ARGV_SIZE 2

#define ELF_MAGIC_SIZE 4
#define ELF_MAGIC_NUMBER 1179403647

#define X86_64 2

#define PAYLOAD_SIZE 54
#define PAGE_SIZE 4096

#define KEY_SIZE 16

#define VOID __attribute__((unused))

////////////////////////////////////////////////////////////////////////////////
/// ENUM
////////////////////////////////////////////////////////////////////////////////

typedef enum
{
	USAGE,
	WRONG_FORMAT,
	WRONG_ARCHITECTURE,
	MALLOC_FAIL,
	WRONG_FD,
	STAT_FAIL,
	MMAP_FAIL,
	CORRUPTION,
} t_error;

////////////////////////////////////////////////////////////////////////////////
/// STRUCTS
////////////////////////////////////////////////////////////////////////////////

typedef struct
{
	uint32_t old_offset;
	uint32_t new_offset;

	uint32_t new_entry;
	uint32_t old_entry;

	uint32_t v_addr;
	uint32_t filesize;

	void *ptr;
	char const *filename;
} t_elf;

typedef struct
{
	t_error const err;
	void (*f)(char const *str);
} t_vector;

////////////////////////////////////////////////////////////////////////////////
/// PROTOTYPES; 
////////////////////////////////////////////////////////////////////////////////

void error(t_error const err, char const *filename);

t_elf *get_elf(char const *filename);
void release_elf(t_elf *elf);

bool is_elf(t_elf const *elf);
bool is_x86(t_elf const *elf);
void *constructor(size_t const size);

void modify_segments(t_elf *elf);
void modify_sections(t_elf const *elf);
void modify_header(t_elf *elf);

void create_infected(t_elf const *elf);

void _memcpy(void *dst, void const *src, size_t const size);
void _memset(void *dst, int const c, const size_t size);
void _xorcpy(void *dst, void const *src, const size_t size, char const *key, const size_t key_size);
char *get_key(const size_t size);

#endif
