# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: joandre- <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/05 18:56:33 by joandre-          #+#    #+#              #
#    Updated: 2024/10/06 05:17:07 by joandre-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.PHONY: token debug
CC = cc
CFLAGS = -Wall -Wextra -Werror
LIBFT = libft/libft.a
NAME = minishell
DBUG = $(shell find debug | grep .c)
TOKN = $(shell find token | grep .c)
SRC = $(TOKN) $(DBUG)
OBJ = $(SRC:.c=.o)


all: $(NAME)

$(NAME): $(LIBFT) $(OBJ)
	$(CC) $(CFLAGS) main.c -l readline $(OBJ) $(LIBFT) -o $(NAME)

$(LIBFT):
	make -s -C libft
	make clean -s -C libft

debug: $(LIBFT) $(OBJ)
	$(CC) $(CFLAGS) dmain.c $(OBJ) $(LIBFT) -o $(NAME)

clean:
	rm -rf $(OBJ) *main.o

fclean: clean
	rm -rf $(NAME)
	make fclean -s -C libft

re: fclean all
