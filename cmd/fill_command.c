/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 02:27:42 by joandre-          #+#    #+#             */
/*   Updated: 2024/12/19 21:09:29 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static bool	fill_data(t_token *token, t_command *cmd, t_data *shell)
{
	char	*str;

	str = expand_path(shell, token->str);
	if (str == NULL)
		return (false);
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

static bool	is_directory(char *str, t_command **command, t_data *shell)
{
	struct stat	data;
	char		*xstr;

	if (!str || !(*str))
		return (false);
	xstr = expand_path(shell, str);
	if (xstr == NULL)
		return (true);
	stat(xstr, ft_memset(&data, 0, sizeof(data)));
	if (isdir_name(xstr) && S_ISDIR(data.st_mode) && !(*command)->error)
	{
		(*command)->error = mini_errmsg(xstr, NULL, "Is a directory", true);
		g_last_exit_code = CMD_NOT_EXECUTABLE;
		return (free(xstr), true);
	}
	if (isdir_name(xstr) && access(xstr, X_OK) != 0 && !(*command)->error)
	{
		(*command)->error = mini_errmsg(xstr, NULL, strerror(errno), true);
		if (access(xstr, F_OK) == -1)
			g_last_exit_code = CMD_NOT_FOUND;
		else
			g_last_exit_code = CMD_NOT_EXECUTABLE;
		return (free(xstr), true);
	}
	return (free(xstr), false);
}

static bool	command_path(t_command **command, t_token *token, t_data *shell)
{
	char	*pwd;

	if (ft_strncmp(token->str, "./", 2) == 0)
	{
		pwd = getcwd(NULL, 0);
		if (pwd == NULL)
			return (perror("minishell"), false);
		(*command)->path = ft_strjoin(pwd, token->str + 1);
		if ((*command)->path == NULL)
			return (free(pwd), perror("minishell"), false);
		free(pwd);
	}
	else
		fill_command_path((*command), shell);
	if (!(*command)->path && !(*command)->error && !(is_builtin((*command))))
	{
		(*command)->error = mini_errmsg((*command)->command, NULL,
				"command not found", true);
		g_last_exit_code = CMD_NOT_FOUND;
		return (false);
	}
	return (true);
}

bool	fill_command(t_command **command, t_token *token, t_data *shell)
{
	if (!command || !(*command) || !token)
		return (false);
	if (is_directory(token->str, command, shell))
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
