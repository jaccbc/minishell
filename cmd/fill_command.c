/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 02:27:42 by joandre-          #+#    #+#             */
/*   Updated: 2024/12/08 02:57:13 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static bool	fill_data(t_token *token, t_command *cmd, t_data *shell)
{
	char	*str;

	str = expand_path(shell->env, token->str);
	if (str == NULL)
		return (mini_errmsg("error", NULL, strerror(errno), false), false);
	if (access(str, F_OK) != -1 && access(str, X_OK) != -1)
	{
		cmd->path = ft_strdup(str);
		if (cmd->path == NULL)
			return (free(str), false);
		cmd->command = ft_strdup(ft_strrchr(cmd->path, '/') + 1);
		if (cmd->command == NULL)
			return (free(str), false);
	}
	else if (cmd->error == NULL)
	{
		cmd->error = mini_errmsg(token->str, NULL, strerror(errno), false);
		return (free(str), false);
	}
	cmd->args = ft_realloc(cmd->args, 2);
	if (cmd->args == NULL)
		return (free(str), false);
	cmd->args[0] = cmd->command;
	cmd->args[1] = NULL;
	return (free(str), true);
}

static bool	isdir_name(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		if (str[i++] == '/')
			return (true);
	return (false);
}

static bool	is_directory(char *str, t_command **command)
{
	struct stat	data;
	bool		dir;

	if (!str || !(*str))
		return (false);
	dir = isdir_name(str);
	lstat(str, &data);
	if (dir && S_ISDIR(data.st_mode) && (*command)->error == NULL)
	{
		(*command)->error = mini_errmsg(str, NULL, "Is a directory", true);
		g_last_exit_code = CMD_NOT_EXECUTABLE;
	}
	if (dir && access(str, X_OK) != 0 && (*command)->error == NULL)
	{
		(*command)->error = mini_errmsg(str, NULL, strerror(errno), true);
		if (access(str, F_OK) == -1)
			g_last_exit_code = CMD_NOT_FOUND;
		else
			g_last_exit_code = CMD_NOT_EXECUTABLE;
	}
	return (false);
}

static bool	command_path(t_command **command, t_token *token, t_data *shell)
{
	if (ft_strncmp(token->str, "./", 2) == 0)
		(*command)->path = ft_strjoin(getenv_path(shell->env, "PWD"), token->str
				+ 1);
	else
		fill_command_path((*command), shell);
	if (!(*command)->path && !(*command)->error && !(is_builtin((*command))))
	{
		(*command)->error = mini_errmsg((*command)->command, NULL,
				"command not found", false);
		g_last_exit_code = CMD_NOT_FOUND;
		return (false);
	}
	return (true);
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
	else if ((*command)->command[0] == '\0')
		return (true);
	(*command)->args = ft_realloc((*command)->args, 2);
	if ((*command)->args == NULL)
		return (false);
	(*command)->args[0] = (*command)->command;
	(*command)->args[1] = NULL;
	if ((*command)->path)
		return (true);
	return (command_path(command, token, shell));
}
