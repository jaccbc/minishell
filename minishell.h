/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 16:27:12 by joandre-          #+#    #+#             */
/*   Updated: 2024/10/28 19:59:18 by vamachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <errno.h>
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
}								t_split;

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
}								t_type;

typedef enum e_redir_type
{
	REDIR_OUT_TRUNC,
	REDIR_OUT_APPEND
}								t_redir_type;

typedef struct s_redirection_node
{
	char						*file_name;
	t_redir_type				type;
	struct s_redirection_node	*next;
}								t_redirection_node;

typedef struct s_redirect
{
	char						*infile;
	char						*outfile;
	t_redir_type				trn_or_app;
	int							fd_in;
	int							fd_out;
}								t_redirect;

typedef struct s_command
{
	char						*command;
	char						**args;
	char						*path;
	bool						has_pipe_output;
	int							pipe_fd[2];
	struct s_redirect			*rdio;
	struct s_command			*next;
	struct s_command			*prev;
}								t_command;

typedef struct s_token
{
	char						*str;
	int							type;
	struct s_token				*prev;
	struct s_token				*next;
	bool						s_quotes;
	bool						d_quotes;
}								t_token;

typedef struct s_data
{
	t_token						*lst;
	t_command					*command;
	char						**env;
}								t_data;

// builtin
int								echo(t_token *lst);
// lexer
t_token							*tokenize(char *s);
t_token							*lstiter_token(t_token *lst, int type,
									size_t i);
void							lstadd_token(t_token **lst, t_token *new);
void							lstdel_token(t_token *lst);
bool							is_type(int type, const char *s);
void							err_msg(char *msg, char *detail,
									bool in_quotes);
bool							syntax_error(t_token *lst);
bool							del_quote(t_token *lst);
void							del_dollar(t_token *lst);
void							var_expander(t_data *shell);
bool							check_syntax(t_data *shell);
bool							validate_redirections(t_data *shell);
void							err_msg_basic(char *filename,
									char *error_message);
// cmd
bool							final_parse(t_data *shell);
t_redirection_node				*create_redirection_node(char *filename,
									t_redir_type type);
void							fill_command_path(t_command *cmd,
									t_data *shell);
void							add_redirection(t_redirection_node **head,
									t_redirection_node *new_node);
void							get_arg(t_command *cmd, t_token *token);
bool							get_cmd(t_command *cmd, t_token *token,
									t_data *shell);
bool							is_redirection(t_token *token);
// utils
char							**ft_realloc(char **array, size_t new_size);
// debug
void							print_list(t_token *lst);
char							*token_name(int type);
void							print_command_list(t_command *cmd_list);

#endif
