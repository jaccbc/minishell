# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: joandre- <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/05 18:56:33 by joandre-          #+#    #+#              #
#    Updated: 2024/10/08 02:29:33 by joandre-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.PHONY: debug
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
	$(CC) $(CFLAGS) dmain.c -l readline $(OBJ) $(LIBFT) -o $(NAME)

norm:
	norminette $(SRC) *.c *.h

clean:
	rm -rf $(OBJ) *.o

fclean: clean
	rm -rf $(NAME)
	make fclean -s -C libft

re: fclean all
