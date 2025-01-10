/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vamachad <vamachad@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 13:48:09 by vamachad          #+#    #+#             */
/*   Updated: 2025/01/10 17:24:42 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Waits for all child processes and returns the last exit code
static int	wait_for_children(t_data *shell)
{
	int	status;
	int	last_exit_code;

	last_exit_code = 0;
	if (waitpid(shell->pid, &status, 0) > 0)
	{
		if (WIFEXITED(status))
			last_exit_code = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			last_exit_code = 128 + WTERMSIG(status);
	}
	while (waitpid(-1, NULL, 0) > 0)
		;
	return (last_exit_code);
}

// Executes the given command in a child process
static int	execute_cmd(t_data *shell, t_command *cmd)
{
	int	ret;

	ret = CMD_NOT_FOUND;
	if (cmd->error != NULL || !cmd->command
		|| (cmd->command && !cmd->command[0]))
	{
		if (cmd->error)
		{
			ft_putendl_fd(cmd->error, STDERR_FILENO);
			ret = shell->status;
		}
		else if (!cmd->command)
			ret = EXIT_FAILURE;
		else if (cmd -> command && !cmd->command[0])
			ret = EXIT_SUCCESS;
		return (lstdel_command(shell->command), free_env(shell->env), ret);
	}
	handle_redirections(cmd);
	ret = execute_builtin(shell, cmd);
	if (ret != CMD_NOT_FOUND)
		return (lstdel_command(shell->command), free_env(shell->env), ret);
	signal(SIGQUIT, SIG_DFL);
	execve(cmd->path, cmd->args, shell->env);
	return (perror("minishell: execve"), lstdel_command(shell->command),
		free_env(shell->env), CMD_NOT_FOUND);
}

static void	handle_child_process(t_command *cmd, t_data *shell)
{
	if (cmd->prev && cmd->prev->pipe_fd[0] != -1)
	{
		dup2(cmd->prev->pipe_fd[0], STDIN_FILENO);
		close(cmd->prev->pipe_fd[0]);
	}
	if (cmd->pipe_fd[1] != -1)
	{
		dup2(cmd->pipe_fd[1], STDOUT_FILENO);
		close(cmd->pipe_fd[1]);
	}
	if (cmd->prev && cmd->prev->pipe_fd[1] != -1)
		close(cmd->prev->pipe_fd[1]);
	if (cmd->pipe_fd[0] != -1)
		close(cmd->pipe_fd[0]);
	exit(execute_cmd(shell, cmd));
}

static int	loop_children(t_data *shell)
{
	t_command	*cmd;
	pid_t		pid;

	cmd = shell->command;
	while (cmd)
	{
		if (cmd->has_pipe_output && pipe(cmd->pipe_fd) == -1)
			return (perror("pipe"), EXIT_FAILURE);
		pid = fork();
		if (pid < 0)
			return (perror("fork"), EXIT_FAILURE);
		if (pid == 0)
			handle_child_process(cmd, shell);
		shell->pid = pid;
		if (cmd->prev && cmd->prev->pipe_fd[0] != -1)
			close(cmd->prev->pipe_fd[0]);
		if (cmd->prev && cmd->prev->pipe_fd[1] != -1)
			close(cmd->prev->pipe_fd[1]);
		cmd = cmd->next;
	}
	return (EXIT_SUCCESS);
}

int	execute(t_data *shell)
{
	t_command	*cmd;
	int			ret;

	cmd = shell->command;
	ret = CMD_NOT_FOUND;
	if (!shell || !cmd || !open_last_red(shell))
		return (EXIT_FAILURE);
	if (!shell->command->has_pipe_output)
	{
		handle_redirections(cmd);
		ret = execute_builtin(shell, cmd);
		restore_red(cmd);
	}
	if (ret != CMD_NOT_FOUND)
		return (ret);
	if (loop_children(shell) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (wait_for_children(shell));
}
