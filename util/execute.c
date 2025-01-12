/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vamachad <vamachad@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 15:13:08 by vamachad          #+#    #+#             */
/*   Updated: 2024/12/04 15:13:10 by vamachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Handles pipes and redirections for the child process
bool	handle_redirections(t_command *cmd)
{
	if (cmd->rdio && cmd->rdio->infile && cmd->rdio->fd_in != -1)
	{
		cmd->rdio->stdin_backup = dup(STDIN_FILENO);
		if (cmd->rdio->stdin_backup == -1)
			return (false);
		dup2(cmd->rdio->fd_in, STDIN_FILENO);
	}
	if (cmd->rdio && cmd->rdio->outfile && cmd->rdio->fd_out != -1)
	{
		cmd->rdio->stdout_backup = dup(STDOUT_FILENO);
		if (cmd->rdio->stdout_backup == -1)
			return (false);
		dup2(cmd->rdio->fd_out, STDOUT_FILENO);
	}
	return (true);
}

bool	restore_red(t_command *cmd)
{
	if (cmd->rdio && cmd->rdio->stdin_backup != -1)
	{
		dup2(cmd->rdio->stdin_backup, STDIN_FILENO);
		close(cmd->rdio->stdin_backup);
		cmd->rdio->stdin_backup = -1;
	}
	if (cmd->rdio && cmd->rdio->stdout_backup != -1)
	{
		dup2(cmd->rdio->stdout_backup, STDOUT_FILENO);
		close(cmd->rdio->stdout_backup);
		cmd->rdio->stdout_backup = -1;
	}
	return (true);
}

// Opens the last redirection infile for each command
bool	open_last_red(t_data *shell)
{
	t_command	*cmd;

	cmd = shell->command;
	while (cmd)
	{
		if (cmd->rdio && cmd->rdio->infile)
			cmd->rdio->fd_in = open(cmd->rdio->infile, O_RDONLY);
		cmd = cmd->next;
	}
	return (true);
}

// Executes a built-in command if found
int	execute_builtin(t_data *shell, t_command *cmd)
{
	if (!cmd->command)
		return (CMD_NOT_FOUND);
	if (ft_strncmp(cmd->command, "echo", 5) == 0)
		return (ft_echo(cmd));
	if (ft_strncmp(cmd->command, "exit", 5) == 0)
		return (ft_exit(shell, cmd));
	if (ft_strncmp(cmd->command, "env", 4) == 0)
		return (ft_env(shell, cmd));
	if (ft_strncmp(cmd->command, "pwd", 4) == 0)
		return (ft_pwd(shell));
	if (ft_strncmp(cmd->command, "unset", 6) == 0)
		return (ft_unset(shell, cmd));
	if (ft_strncmp(cmd->command, "export", 7) == 0)
		return (ft_export(shell, cmd));
	if (ft_strncmp(cmd->command, "cd", 3) == 0)
		return (ft_cd(shell, cmd));
	return (CMD_NOT_FOUND);
}
