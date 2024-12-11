/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 16:27:12 by joandre-          #+#    #+#             */
/*   Updated: 2024/12/11 22:26:14 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/wait.h>

# define PROMPT "\033[1;34mMINISHELL$ \033[0m"

extern int				g_last_exit_code;

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
}						t_split;

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
}						t_type;

typedef struct s_redirect
{
	char				*infile;
	char				*outfile;
	bool				heredoc;
	int					fd_in;
	int					fd_out;
	int					stdin_backup;
	int					stdout_backup;
}						t_redirect;

typedef struct s_command
{
	char				*command;
	char				*error;
	char				**args;
	char				*path;
	bool				has_pipe_output;
	int					*pipe_fd;
	t_redirect			*rdio;
	struct s_command	*next;
	struct s_command	*prev;
}						t_command;

typedef struct s_token
{
	char				*str;
	int					type;
	struct s_token		*prev;
	struct s_token		*next;
	bool				s_quotes;
	bool				d_quotes;
}						t_token;

typedef struct s_data
{
	char				*user_input;
	t_token				*lst;
	t_command			*command;
	char				**env;
	pid_t				pid;
	char				*working_dir;
	char				*old_working_dir;
}						t_data;

// builtin
int			ft_echo(t_command *cmd);
int			ft_exit(t_data *shell, t_command *cmd);
int			ft_env(t_data *shell, t_command *cmd);
int			ft_pwd(t_data *shell);
int			ft_unset(t_data *shell, t_command *cmd);
int			ft_export(t_data *shell, t_command *cmd);
int			ft_cd(t_data *shell, t_command *cmd);
// utils
void		cd_errmsg(char *msg, int err);
bool		update_env(char *path, const char *var, t_data *shell);
bool		ft_strcmp(char *s1, char *s2);
char		*mini_errmsg(char *command, char *detail,
				char *error_message, bool prt_mini);
char		**ft_realloc(char **array, size_t new_size);
void		lstdel_command(t_command *cmd);
void		add_command_back(t_command **cmd, t_command *new);
char		*getenv_path(char **env, const char *var);
char		*expand_path(char **env, char *str);
bool		is_builtin(t_command *cmd);
bool		handle_pipes_and_redirections(t_command *cmd);
bool		restore_red(t_command *cmd);
void		close_unused_pipes(t_command *cmd);
bool		piping(t_data *shell);
bool		open_last_red(t_data *shell);
void		exit_cleanup(t_data *shell);
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
void		sighandler_noninteractive(void);
// execute
int			execute(t_data *shell);
void		free_env(char **env);
// debug
void		print_list(t_token *lst);
char		*token_name(int type);
void		print_command_list(t_command *cmd_list);

#endif
