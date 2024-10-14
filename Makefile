# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: joandre- <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/05 18:56:33 by joandre-          #+#    #+#              #
#    Updated: 2024/10/13 01:15:36 by joandre-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.PHONY: debug
CC = cc
CFLAGS = -Wall -Wextra -Werror
LIBFT = libft/libft.a
NAME = minishell
DBG = $(shell find debug -name "*.c")
LEX = $(shell find lexer -name "*.c")
BLT = $(shell find builtin -name "*.c")
SRC = $(BLT) $(LEX) $(DBG)
OBJ = $(SRC:.c=.o)


all: $(NAME)

$(NAME): $(LIBFT) $(OBJ)
	$(CC) $(CFLAGS) main.c -l readline $(OBJ) $(LIBFT) -o $(NAME)

$(LIBFT):
	make -s -C libft
	make clean -s -C libft

debug: $(LIBFT) $(OBJ)
	$(CC) $(CFLAGS) dmain.c -l readline $(OBJ) $(LIBFT) -o $(NAME)

norm:
	norminette $(SRC) *.c *.h

clean:
	rm -rf $(OBJ) *.o

fclean: clean
	rm -rf $(NAME)
	make fclean -s -C libft

re: fclean all
