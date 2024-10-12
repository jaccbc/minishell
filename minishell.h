/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 16:27:12 by joandre-          #+#    #+#             */
/*   Updated: 2024/10/12 00:50:12 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <string.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include "libft/libft.h"

# define PROMPT "MINISHELL>"

typedef enum token_utils
{
	QUOTE,
	UNQUOTE,
	DELIMIT,
	UP,
	DOWN
}	t_split;

typedef enum token_type
{
	INVALID,
	COMMAND,
	FLAG,
	ARG,
	VAR,
	RED_IN,
	RED_OUT,
	PIPE,
	APPEND,
	HEREDOC
}	t_type;

typedef struct s_token
{
	char			*str;
	int				type;
	struct s_token	*prev;
	struct s_token	*next;
}	t_token;

typedef struct s_data
{
	t_token	*lst;
}	t_data;

//builtin
int		echo(t_token *lst);
//token
t_token	*tokenize(char *s);
t_token	*lstiter_token(t_token *lst, int type, size_t i);
void	lstadd_token(t_token **lst, t_token *new);
void	free_token(t_token *lst);
bool	is_type(int type, char *s);
//debug
void	print_list(t_token *lst);
char	*token_name(int type);

#endif
