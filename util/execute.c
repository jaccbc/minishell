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

void	close_pipe_fds(t_command *cmds, t_command *skip_cmd)
{
	while (cmds)
	{
		if (cmds != skip_cmd && cmds->pipe_fd)
		{
			close(cmds->pipe_fd[0]);
			close(cmds->pipe_fd[1]);
		}
		cmds = cmds->next;
	}
}

// Handles pipes and redirections for the child process
bool	handle_pipes_and_redirections(t_command *cmds, t_command *cmd)
{
	if (cmd->rdio && cmd->rdio->infile && cmd->rdio->fd_in != -1)
	{
		cmd->rdio->stdin_backup = dup(STDIN_FILENO);
		if (cmd->rdio->stdin_backup == -1)
			return (false);
		dup2(cmd->rdio->fd_in, STDIN_FILENO);
	}
	else if (cmd->prev && cmd->prev->has_pipe_output)
		dup2(cmd->prev->pipe_fd[0], STDIN_FILENO);
	if (cmd->rdio && cmd->rdio->outfile && cmd->rdio->fd_out != -1)
	{
		cmd->rdio->stdout_backup = dup(STDOUT_FILENO);
		if (cmd->rdio->stdout_backup == -1)
			return (false);
		dup2(cmd->rdio->fd_out, STDOUT_FILENO);
	}
	else if (cmd->has_pipe_output)
		dup2(cmd->pipe_fd[1], STDOUT_FILENO);
	if (cmds->has_pipe_output)
		close_pipe_fds(cmds, cmd);
	if (cmd->has_pipe_output)
		close(cmd->pipe_fd[0]);
	if (cmd->prev && cmd->prev->has_pipe_output)
		close(cmd->prev->pipe_fd[1]);
	return (true);
}

bool	restore_red(t_command *cmd)
{
	if (cmd->rdio && cmd->rdio->stdin_backup != -1)
	{
		dup2(cmd->rdio->stdin_backup, STDIN_FILENO);
		cmd->rdio->stdin_backup = -1;
	}
	if (cmd->rdio && cmd->rdio->stdout_backup != -1)
	{
		dup2(cmd->rdio->stdout_backup, STDOUT_FILENO);
		cmd->rdio->stdout_backup = -1;
	}
	return (true);
}

// Creates pipes for commands with pipe output
bool	piping(t_data *shell)
{
	t_command	*cmd;

	cmd = shell->command;
	while (cmd)
	{
		if (cmd->has_pipe_output)
		{
			cmd->pipe_fd = malloc(sizeof(int) * 2);
			if (!cmd->pipe_fd || pipe(cmd->pipe_fd) == -1)
				return (false);
		}
		cmd = cmd->next;
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
