/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 01:20:35 by joandre-          #+#    #+#             */
/*   Updated: 2024/12/04 13:16:17 by vamachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**ft_realloc(char **array, size_t new_size)
{
	char	**new_array;
	size_t	i;

	new_array = malloc(sizeof(char *) * new_size);
	if (!new_array)
		return (NULL);
	i = 0;
	while (array && array[i] && i < new_size - 1)
	{
		new_array[i] = array[i];
		i++;
	}
	while (i < new_size - 1)
		new_array[i++] = NULL;
	free(array);
	return (new_array);
}

// Add a command node to the back of the command list
void	add_command_back(t_command **head, t_command *new)
{
	t_command	*last;

	if (!head || !new)
		return ;
	if (!*head)
		*head = new;
	else
	{
		last = *head;
		while (last->next)
			last = last->next;
		last->next = new;
		new->prev = last;
	}
}

char	*expand_path(char **env, char *str)
{
	char	*pwd;
	int		i;

	pwd = getenv_path(env, "PWD");
	if (pwd == NULL)
		return (ft_putendl_fd("PWD not set", STDERR_FILENO), NULL);
	if (ft_strncmp(str, "./", 2) == 0)
		return (ft_strjoin(getenv_path(env, "PWD"), str + 1));
	if (ft_strncmp(str, "~/", 2) == 0)
		return (ft_strjoin(getenv_path(env, "HOME"), str + 1));
	i = ft_strlen(pwd);
	if (ft_strncmp(str, "../", 3) == 0)
	{
		while (ft_strncmp(str, "../", 3) == 0)
		{
			str += 3;
			while (i)
			{
				if (pwd[--i] == '/')
					break ;
			}
		}
		pwd[i + 1] = '\0';
		return (ft_strjoin(pwd, str));
	}
	if (ft_strcmp(str, ".."))
	{
		while (i)
		{
			if (pwd[--i] == '/')
			{
				if (i == 0)
					pwd[i + 1] = '\0';
				else
					pwd[i] = '\0';
				break ;
			}
		}
		return (ft_strdup(pwd));
	}
	return (ft_strdup(str));
}

static bool	fill_data(t_token *token, t_command *cmd, t_data *shell)
{
	char	*str;

	str = expand_path(shell->env, token->str);
	if (str == NULL)
		return (mini_errmsg("error", NULL, strerror(errno), false), false);
	if (access(str, F_OK) != -1 && access(str, X_OK) != -1)
	{
		cmd->path = ft_strdup(str);
		free(str);
		if (cmd->path == NULL)
			return (false);
		cmd->command = ft_strdup(ft_strrchr(cmd->path, '/') + 1);
		if (cmd->command == NULL)
			return (false);
	}
	else if (cmd->error == NULL)
	{
		cmd->error = mini_errmsg(token->str, NULL, strerror(errno), false);
		free(str);
		return (false);
	}
	cmd->args = ft_realloc(cmd->args, 2);
	if (cmd->args == NULL)
		return (false);
	cmd->args[0] = cmd->command;
	cmd->args[1] = NULL;
	return (true);
}

static bool	is_directory(char *str, t_command **command)
{
	struct stat	data;
	int			i;
	bool		dir;

	if (!str || !(*str))
		return (false);
	dir = false;
	i = -1;
	while (str[++i])
	{
		if (str[i] == '/')
		{
			dir = true;
			break ;
		}
	}
	if (dir && lstat(str, &data) == 0)
	{
		if ((*command)->error == NULL && S_ISDIR(data.st_mode))
		{
			(*command)->error = mini_errmsg(str, NULL, "Is a directory", true);
			g_last_exit_code = CMD_NOT_EXECUTABLE;
		}
		if ((*command)->error == NULL && S_ISLNK(data.st_mode))
		{
			(*command)->error = mini_errmsg(str, NULL, "Permission denied",
					true);
			g_last_exit_code = CMD_NOT_EXECUTABLE;
		}
		if ((*command)->error == NULL && access(str, F_OK) == -1)
		{
			(*command)->error = mini_errmsg(str, NULL, strerror(errno), true);
			g_last_exit_code = CMD_NOT_FOUND;
		}
	}
	if ((*command)->error)
		return (true);
	return (false);
}

// Get the command string and initialize args with command name
bool	fill_command(t_command **command, t_token *token, t_data *shell)
{
	if (!command || !(*command) || !token)
		return (false);
	if (is_directory(token->str, command))
		return (false);
	if (is_type(PATH, token->str))
		return (fill_data(token, *command, shell));
	else
		(*command)->command = ft_strdup(token->str);
	if ((*command)->command == NULL)
		return (false);
	(*command)->args = ft_realloc((*command)->args, 2);
	if ((*command)->args == NULL)
		return (false);
	(*command)->args[0] = (*command)->command;
	(*command)->args[1] = NULL;
	if ((*command)->path)
		return (true);
	if (ft_strncmp(token->str, "./", 2) == 0)
		(*command)->path = ft_strjoin(getenv_path(shell->env, "PWD"), token->str
				+ 1);
	else
		fill_command_path((*command), shell);
	if (!(*command)->path && !(*command)->error)
	{
		(*command)->error = mini_errmsg((*command)->command, NULL,
				"command not found", false);
		return (false);
	}
	return (true);
}

// Add argument to the t_command struct's args field
bool	fill_args(t_command **command, t_token *token)
{
	t_command	*cmd;
	int			i;

	i = 0;
	if (!command || !token)
		return (false);
	cmd = *command;
	if (cmd->args)
		while (cmd->args[i])
			i++;
	cmd->args = ft_realloc(cmd->args, i + 2);
	if (cmd->args == NULL)
		return (false);
	cmd->args[i] = ft_strdup(token->str);
	if (cmd->args[i] == NULL)
		return (false);
	cmd->args[i + 1] = NULL;
	return (true);
}

static void	del_redirect(t_redirect *rdio)
{
	if (rdio)
	{
		if (rdio->heredoc)
			unlink(rdio->infile);
		free(rdio->infile);
		free(rdio->outfile);
		if (rdio->fd_in != -1)
			close(rdio->fd_in);
		if (rdio->fd_out != -1)
			close(rdio->fd_out);
		free(rdio);
	}
}

void	lstdel_command(t_command *lst)
{
	t_command	*del;
	int			i;

	while (lst)
	{
		if (lst->error)
			free(lst->error);
		if (lst->args)
		{
			i = -1;
			while (lst->args[++i])
				free(lst->args[i]);
			free(lst->args);
		}
		if (lst->path)
			free(lst->path);
		if (lst->pipe_fd)
			free(lst->pipe_fd);
		del_redirect(lst->rdio);
		del = lst;
		lst = lst->next;
		free(del);
	}
}

bool	is_builtin(t_command *cmd)
{
	if (!cmd->command)
		return (false);
	if (ft_strncmp(cmd->command, "echo", 5) == 0)
		return (true);
	if (ft_strncmp(cmd->command, "exit", 5) == 0)
		return (true);
	if (ft_strncmp(cmd->command, "env", 4) == 0)
		return (true);
	if (ft_strncmp(cmd->command, "pwd", 4) == 0)
		return (true);
	if (ft_strncmp(cmd->command, "unset", 6) == 0)
		return (true);
	if (ft_strncmp(cmd->command, "export", 7) == 0)
		return (true);
	if (ft_strncmp(cmd->command, "cd", 3) == 0)
		return (true);
	return (false);
}
