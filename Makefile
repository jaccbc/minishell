# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vamachad <vamachad@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/05 18:56:33 by joandre-          #+#    #+#              #
#    Updated: 2025/01/06 13:32:22 by vamachad         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.PHONY: debug
CC = cc
CFLAGS = -Wall -Wextra -Werror
LIBFT = libft/libft.a
NAME = minishell
DBG = $(shell find debug -name "*.c")
LEX = lexer/check_syntax.c lexer/del_quote.c lexer/error.c lexer/tokenize.c
BLT = builtin/cd.c builtin/echo.c builtin/env.c builtin/exit.c builtin/export.c builtin/pwd.c builtin/unset.c
VAR = var/expansion.c
CMD = cmd/check_files.c cmd/cmd.c cmd/fill_command.c cmd/parse_heredoc.c cmd/path.c
SIG = signal/sighandler.c
UTL = util/builtin.c util/command.c util/execute.c util/general.c util/token.c
EXC = execute/exec.c
SRC = $(UTL) $(BLT) $(LEX) $(DBG) $(VAR) $(CMD) $(SIG) $(EXC)
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
	norminette $(SRC)

run: $(NAME)
	valgrind --leak-check=full --show-leak-kinds=all --suppressions=valgrind_readline.supp ./minishell

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)
	make fclean -s -C libft

re: fclean all

