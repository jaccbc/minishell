# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vamachad <vamachad@student.42lisboa.com>   +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/04 21:50:59 by vamachad          #+#    #+#              #
#    Updated: 2024/05/05 00:20:49 by vamachad         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = 	libftprintf.a

SRC = 	ft_printf.c \
	print_address.c \
	print_nbr.c \
	print_string.c \
	print_char.c \
	print_unsigned.c

OBJS =	$(SRC:.c=.o)

CC = cc	
CFLAGS = -Wall -Werror -Wextra
RM = rm -rf
AR = ar rcs

$(NAME): $(OBJS)
	$(AR) $(NAME) $(OBJS)

all: $(NAME)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
