# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jle-quel <jle-quel@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/12/07 10:51:42 by jle-quel          #+#    #+#              #
#    Updated: 2018/12/14 15:51:39 by jle-quel         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# **************************************************************************** #
# INIT
# **************************************************************************** #

NAME = woody_woodpacker
INFECTED = woody

SRC_PATH = src
OBJ_PATH = obj
INC_PATH = inc

SRC_NAME =	main.c			\
			woody.c			\
			error.c			\
			utils.c			\
			elf.c			\
			segment.c		\
			section.c		\
			header.c		\
			infection.c		\

OBJ_NAME = $(SRC_NAME:.c=.o)

CC = clang
CFLAGS = -Wall -Wextra -Werror -g3 -fsanitize=address

RED=[1;31m
GREEN=[1;32m
NC=[0m

SRC = $(addprefix $(SRC_PATH)/,$(SRC_NAME))
OBJ = $(addprefix $(OBJ_PATH)/,$(OBJ_NAME))

.PHONY: all, clean, fclean, re

# **************************************************************************** #
# COMPILATION 
# **************************************************************************** #

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) $^ -o $@

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c Makefile
	@mkdir $(OBJ_PATH) 2> /dev/null || true
	@$(CC) $(CFLAGS) -c $< -o $@ -I$(INC_PATH)
	@echo "$(GREEN)[✓]$(NC) Source compiled : $<"

clean:
	@rm -f $(OBJ)
	@rmdir $(OBJ_PATH) 2> /dev/null || true
	@echo "$(RED)[-]$(NC) Objects cleaned"

fclean: clean
	@rm -rf $(NAME)
	@rm -rf $(INFECTED)
	@echo "$(RED)[-]$(NC) Program clear"

re:fclean
	@make

