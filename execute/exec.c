/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vamachad <vamachad@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 13:48:09 by vamachad          #+#    #+#             */
/*   Updated: 2025/01/09 12:25:02 by joandre-         ###   ########.fr       */
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

// Executes a built-in command if found
static int	execute_builtin(t_data *shell, t_command *cmd)
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
	handle_pipes_and_redirections(cmd);
	ret = execute_builtin(shell, cmd);
	if (ret != CMD_NOT_FOUND)
		return (lstdel_command(shell->command), free_env(shell->env), ret);
	execve(cmd->path, cmd->args, shell->env);
	perror("minishell: execve");
	return (lstdel_command(shell->command),
		free_env(shell->env), CMD_NOT_FOUND);
}

static int	loop_children(t_data *shell)
{
	t_command	*cmd;
	pid_t		pid;

	cmd = shell->command;
	while (cmd)
	{
		pid = fork();
		if (pid < 0)
		{
			perror("fork");
			return (EXIT_FAILURE);
		}
		else if (pid == 0)
			exit(execute_cmd(shell, cmd));
		else
		{
			close_unused_pipes(cmd);
			shell->pid = pid;
		}
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
	if (!shell || !cmd || !piping(shell) || !open_last_red(shell))
		return (EXIT_FAILURE);
	if (!shell->command->has_pipe_output)
	{
		handle_pipes_and_redirections(cmd);
		ret = execute_builtin(shell, cmd);
		restore_red(cmd);
	}
	if (ret != CMD_NOT_FOUND)
		return (ret);
	if (loop_children(shell) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (wait_for_children(shell));
}
