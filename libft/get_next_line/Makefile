# Compiler and compiling flags
CC = cc
CFLAGS = -Wall -Wextra -Werror

# Name of the executable
NAME = gnl

# Source and object files
SRC = get_next_line.c get_next_line_utils.c
OBJ = $(SRC:.c=.o)

# Default rule
all: $(NAME)

# Rule to make the executable
$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ)

# Rule for object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to clean object files
clean:
	rm -f $(OBJ)

# Rule to clean everything
fclean: clean
	rm -f $(NAME)

# Rule to re-make everything
re: fclean all

# Phony targets
.PHONY: all clean fclean re