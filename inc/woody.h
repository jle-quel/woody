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

#define VALID_AV_SIZE 2

#define ELF_MAGIC_SIZE 4
#define ELF_MAGIC_NUMBER 1179403647
#define PACK_MAGIC_NUMBER 0x15D25

#define X86_64 2

#define PAYLOAD_SIZE 94
#define PAGE_SIZE 4096

#define DEFAULT_SIZE 16

#define VOID __attribute__((unused))

////////////////////////////////////////////////////////////////////////////////
/// ENUM
////////////////////////////////////////////////////////////////////////////////

typedef enum
{
	USAGE,
	WRONG_FORMAT,
	WRONG_ARCHITECTURE,
	NOT_EXEC,
	MALLOC_FAIL,
	WRONG_FD,
	STAT_FAIL,
	MMAP_FAIL,
	CORRUPTION,
	PACKED,
} t_error;

////////////////////////////////////////////////////////////////////////////////
/// STRUCTS
////////////////////////////////////////////////////////////////////////////////

typedef struct
{
	Elf64_Addr old_entrypoint;

	Elf64_Off segment_offset;
	Elf64_Addr segment_addr;
	Elf64_Xword segment_size;

	Elf64_Off section_offset;
	Elf64_Addr section_addr;
	Elf64_Xword section_size;

	void *ptr;
	size_t filesize;
	char const *filename;
} t_elf;

////////////////////////////////////////////////////////////////////////////////
/// PROTOTYPES; 
////////////////////////////////////////////////////////////////////////////////

void woody(char const *filename);

void error(t_error const err, char const *filename);

t_elf *get_elf(char const *filename);
void release_elf(t_elf *elf);

void modify_segments(t_elf *elf);
void modify_sections(t_elf *elf);
void modify_header(t_elf *elf);
void create_infected(t_elf const *elf);

void *constructor(size_t const size);
void _memcpy(void *dst, void const *src, size_t const size);
void _memset(void *dst, int const c, const size_t size);

char *generate_key(const size_t size);

#endif
