# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vamachad <vamachad@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/05 18:56:33 by joandre-          #+#    #+#              #
#    Updated: 2025/01/08 17:50:53 by joandre-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS = -Wall -Wextra -Werror
LIBFT = libft/libft.a
NAME = minishell
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

run: $(NAME)
	valgrind -s --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=valgrind_readline.supp ./$(NAME)

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)
	make fclean -s -C libft

re: fclean all

rerun: re run
