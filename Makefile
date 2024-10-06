# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: joandre- <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/05 18:56:33 by joandre-          #+#    #+#              #
#    Updated: 2024/10/06 03:02:06 by joandre-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.PHONY: debug
CC = cc
CFLAGS = -Wall -Wextra -Werror
LIBFT = libft/libft.a
NAME = minishell
SRC = token.c token_utils.c
OBJ = $(SRC:.c=.o)
DBUG_SRC = debug/debug.c
DBUG_OBJ = $(DBUG_SRC:.c=.o)


all: $(NAME)

$(NAME): $(LIBFT) $(OBJ) $(DBUG_OBJ)
	$(CC) $(CFLAGS) main.c -l readline $(DBUG_OBJ) $(OBJ) $(LIBFT) -o $(NAME)

$(LIBFT):
	make -s -C libft
	make clean -s -C libft

debug: $(LIBFT) $(DBUG_OBJ)
	$(CC) $(CFLAGS) debug/dmain.c $(DBUG_OBJ) $(OBJ) $(LIBFT) -o $(NAME)

clean:
	rm -rf $(OBJ) $(DBUG_OBJ) main.o debug/dmain.o

fclean: clean
	rm -rf $(NAME)
	make fclean -s -C libft

re: fclean all
