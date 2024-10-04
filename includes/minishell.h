#ifndef MINISHELL_H
# define MINISHELL_H

# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>

#include "../libft/libft.h"

#  define PROMPT "MINISHELL>"

typedef struct	s_token_list
{
	char	*str;
	int	type;
	struct s_token_list	*next;
	struct s_token_list	*prev;
}	token_list;

typedef	struct	s_data
{
	token_list	*token;
	char	**token_arr;

}		mini_data;

typedef enum {
    TOKEN_COMMAND = 1,
    TOKEN_FLAG,
    TOKEN_PIPE,
    TOKEN_REDIRECTION_OUT,
    TOKEN_REDIRECTION_IN,
    TOKEN_APPEND_OUT,
    TOKEN_UNKNOWN
} token_type;

// Function prototypes
int		tokenization(char *s);
token_list	*ft_new_token(char *s, int type);
void		ft_lstadd_token(token_list **lst, token_list *new);
int			free_lst(token_list **lst);
void		print_list(token_list *lst);
int			token_array(char *s);

#endif