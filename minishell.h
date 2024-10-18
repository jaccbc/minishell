/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 16:27:12 by joandre-          #+#    #+#             */
/*   Updated: 2024/10/18 03:18:30 by vamachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# define PROMPT "MINISHELL$ "

typedef enum token_utils
{
	QUOTE,
	UNQUOTE,
	DELIMIT,
	UP,
	DOWN
}					t_split;

typedef enum token_types
{
	INVALID,
	COMMAND,
	FLAG,
	ARG,
	VAR,
	PIPE,
	RED_IN,
	RED_OUT,
	APPEND,
	HEREDOC
}					t_type;

typedef struct s_token
{
	char			*str;
	int				type;
	bool			var_in_d_quotes;
	bool			s_quotes;
	bool			d_quotes;
	struct s_token	*prev;
	struct s_token	*next;
}					t_token;

typedef struct s_data
{
	t_token			*lst;
	char			**env;
}					t_data;

// builtin
int					echo(t_token *lst);
// lexer
t_token				*tokenize(char *s);
t_token				*lstiter_token(t_token *lst, int type, size_t i);
void				lstadd_token(t_token **lst, t_token *new);
void				free_token(t_token *lst);
bool				is_type(int type, char *s);
void				err_msg(char *msg, char *detail, bool in_quotes);
bool				syntax_error(t_token *lst);
bool				check_syntax(t_data *shell);
// var
void				var_expander(t_data *shell);
bool				there_is_dollar(char *str);
bool				is_in_s_quotes(char *str, bool s_quotes);
bool				nxt_ch_sep(char c);
bool				var_in_quotes(char *str);
char				*get_value(t_data *shell, char *str);
void				sub_var(t_token *current, int i, char *value);
int					var_length(char *str);
char				*extract_var_name(char *s);
int					erase_var(t_token *lst, int index);
void				copy_value(char *new_str, int *j, char *value);
int					erase_and_replace_var(t_token *lst, int index, char *value);
bool				is_var_compliant(char c);
void				free_ptr(void *ptr);
// debug
void				print_list(t_token *lst);
char				*token_name(int type);
void				print_array(char **arr);

#endif
