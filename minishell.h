/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 16:27:12 by joandre-          #+#    #+#             */
/*   Updated: 2024/11/11 00:38:48 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <errno.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <sys/stat.h>
# include <stdbool.h>
# include <string.h>
# include <signal.h>
# include <sys/wait.h>
# include <fcntl.h>

# define PROMPT "MINISHELL$ "

extern int	g_last_exit_code;

# define CMD_NOT_FOUND 127
# define CMD_NOT_EXECUTABLE 126

typedef enum token_utils
{
	QUOTE,
	UNQUOTE,
	DELIMIT,
	UP,
	DOWN,
	PATH
}								t_split;

typedef enum token_types
{
	INVALID,
	COMMAND,
	ARG,
	VAR,
	PIPE,
	RED_IN,
	RED_OUT,
	APPEND,
	HEREDOC
}								t_type;

typedef struct s_redirect
{
	char					*infile;
	char					*outfile;
	bool					heredoc;
	int						fd_in;
	int						fd_out;
	int						stdin_backup;
	int						stdout_backup;
}							t_redirect;

typedef struct s_command
{
	char					*command;
	char					*error;
	char					**args;
	char					*path;
	bool					has_pipe_output;
	int						*pipe_fd;
	t_redirect				*rdio;
	struct s_command		*next;
	struct s_command		*prev;
}							t_command;

typedef struct s_token
{
	char					*str;
	int						type;
	struct s_token			*prev;
	struct s_token			*next;
	bool					s_quotes;
	bool					d_quotes;
}							t_token;

typedef struct s_data
{
	char					*user_input;
	t_token					*lst;
	t_command				*command;
	char					**env;
	pid_t					pid;
}							t_data;

// builtin
int			ft_echo(t_command *cmd);
int			ft_exit(t_data *shell);
// utils
bool		ft_strcmp(char *s1, char *s2);
char		*minishell_errmsg(char *filename, char *error_message);
char		**ft_realloc(char **array, size_t new_size);
void		lstdel_command(t_command *cmd);
void		add_command_back(t_command **cmd, t_command *new);

// lexer
t_token		*tokenize(char *s);
t_token		*lstiter_token(t_token *lst, int type, size_t i);
void		lstadd_token(t_token **lst, t_token *new);
void		lstdel_token(t_token *lst);
bool		is_type(int type, const char *s);
bool		syntax_error(t_token *lst);
bool		del_quote(t_token *lst);
void		del_dollar(t_token *lst);
void		expander(char **s, char **env, char *var);
bool		check_syntax(t_data *shell);
bool		check_files(t_token *lst, t_command **cmd, char **env);
// cmd
t_redirect	*create_redirect(void);
bool		final_parse(t_data *shell);
void		fill_command_path(t_command *cmd, t_data *shell);
bool		fill_args(t_command **cmd, t_token *token);
bool		fill_command(t_command **cmd, t_token *token, t_data *shell);
bool		parse_heredoc(t_redirect *rdio, t_token *lst, char **env);
// signal
bool		sighandler(void);
//execute
int			execute(t_data *shell);
// debug
void		print_list(t_token *lst);
char		*token_name(int type);
void		print_command_list(t_command *cmd_list);

#endif
