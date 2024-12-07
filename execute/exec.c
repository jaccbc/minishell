/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vamachad <vamachad@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 13:48:09 by vamachad          #+#    #+#             */
/*   Updated: 2024/12/04 13:48:13 by vamachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Waits for all child processes and returns the last exit code
static int	wait_for_children(void)
{
	int	status;
	int	last_exit_code;

	last_exit_code = 0;
	while (waitpid(-1, &status, 0) > 0)
	{
		if (WIFEXITED(status))
			last_exit_code = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			last_exit_code = 128 + WTERMSIG(status);
	}
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
		return (ft_cd(cmd, shell->env));
	return (CMD_NOT_FOUND);
}


// Executes the given command in a child process
static int	execute_cmd(t_data *shell, t_command *cmd)
{
	int	ret;

	if (cmd->error != NULL || !cmd->command || !cmd->command[0])
	{
		if (cmd->error)
		{
			ft_putendl_fd(cmd->error, STDERR_FILENO);
			ret = g_last_exit_code;
		}
		/* else if (!cmd->command)
			ret = EXIT_FAILURE; */ //este acho que não faz
		else if (!cmd->command[0])
			ret = EXIT_SUCCESS;
		lstdel_command(shell->command);
		free_env(shell->env);
		return (ret);
	}
	handle_pipes_and_redirections(cmd);
	ret = execute_builtin(shell, cmd);
	if (ret != CMD_NOT_FOUND)
	{
		lstdel_command(shell->command);
		free_env(shell->env);
		return (ret);
	}
	execve(cmd->path, cmd->args, shell->env);
	perror("execve");
	return (CMD_NOT_FOUND);
}

static int	loop_children(t_data *shell)
{
	t_command	*cmd;

	cmd = shell->command;
	while (cmd)
	{
		shell->pid = fork();
		if (shell->pid == 0)
			exit(execute_cmd(shell, cmd));
		else if (shell->pid > 0)
			close_unused_pipes(cmd);
		else
		{
			perror("fork");
			return (EXIT_FAILURE);
		}
		g_last_exit_code = wait_for_children();
		cmd = cmd->next;
	}
	return (EXIT_SUCCESS);
}

// Main execute function to fork processes and manage pipes
int	execute(t_data *shell)
{
	t_command	*cmd;

	cmd = shell->command;
	if (!shell || !cmd || !piping(shell) || !open_last_red(shell))
		return (EXIT_FAILURE);
	if (!shell->command->has_pipe_output)
	{
		handle_pipes_and_redirections(cmd);
		g_last_exit_code = execute_builtin(shell, cmd);
		restore_red(cmd);
	}
	if (g_last_exit_code != CMD_NOT_FOUND)
		return (g_last_exit_code);
	if (loop_children(shell) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (g_last_exit_code);
}
