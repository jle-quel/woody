################################################################################
### INIT
################################################################################

NAME		= woody_woodpacker
INFECTED	= woody

SRC_PATH_C	= src/c
SRC_PATH_S	= src/asm

OBJ_PATH_C	= obj/c
OBJ_PATH_S	= obj/asm

INC_PATH	= inc

CC		= gcc
AS		= nasm
CFLAGS		= -Wall -Wextra -Werror -I $(INC_PATH)
ASFLAGS		= -f elf64

################################################################################
### OBJECTS 
################################################################################

OBJ_NAME_C	=	main.o						\
			woody.o						\
			key.o						\
			error.o						\
			utils.o						\
			elf.o						\
			segment.o					\
			section.o					\
			header.o					\
			infection.o					\

OBJ_NAME_S	=	rc4.o						\

OBJ		=	$(addprefix $(OBJ_PATH_C)/,$(OBJ_NAME_C))	\
			$(addprefix $(OBJ_PATH_S)/,$(OBJ_NAME_S))

################################################################################
### RULES
################################################################################

.PHONY: all, clean, fclean, re

all: obj $(NAME)

obj:
	mkdir -p obj/c obj/asm

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ_PATH_C)/%.o: $(SRC_PATH_C)/%.c Makefile
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_PATH_S)/%.o: $(SRC_PATH_S)/%.s Makefile
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf obj
	rm -rf $(NAME)
	rm -rf $(INFECTED)

re: fclean all
