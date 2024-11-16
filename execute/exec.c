#include "../minishell.h"

// Handles pipes and redirections for the child process
void	handle_pipes_and_redirections(t_command *cmd)
{
	if (cmd->rdio && cmd->rdio->infile && cmd->rdio->fd_in != -1)
		dup2(cmd->rdio->fd_in, STDIN_FILENO);
	else if (cmd->prev && cmd->prev->has_pipe_output)
		dup2(cmd->prev->pipe_fd[0], STDIN_FILENO);
	if (cmd->rdio && cmd->rdio->outfile && cmd->rdio->fd_out != -1)
		dup2(cmd->rdio->fd_out, STDOUT_FILENO);
	else if (cmd->has_pipe_output)
		dup2(cmd->pipe_fd[1], STDOUT_FILENO);

	// Close all pipe ends after duplicating
	if (cmd->has_pipe_output)
		close(cmd->pipe_fd[0]);
	if (cmd->prev && cmd->prev->has_pipe_output)
		close(cmd->prev->pipe_fd[1]);
}

// Closes unused pipe file descriptors in the parent process
void	close_unused_pipes(t_command *cmd)
{
	if (cmd->has_pipe_output)
		close(cmd->pipe_fd[1]); // Parent closes write end
	if (cmd->prev && cmd->prev->has_pipe_output)
		close(cmd->prev->pipe_fd[0]); // Parent closes read end of the previous pipe
}

// Waits for all child processes and returns the last exit code
int	wait_for_children(void)
{
	int	status;
	int	last_exit_code;

	last_exit_code = 0;
	while (waitpid(-1, &status, 0) > 0)
	{
		if (WIFEXITED(status))
			last_exit_code = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			int sig = WTERMSIG(status);
			if (sig == SIGINT)
				printf("\n"); // Handle Ctrl+C
			else if (sig == SIGSEGV)
				printf("Segmentation fault (core dumped)\n");
			else if (sig == SIGTERM)
				printf("Terminated\n");
			else
				printf("Killed by signal %d\n", sig); // Fallback
		}
	}
	return (last_exit_code);
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

// Executes a built-in command if found
/* int	execute_builtin(t_data *shell, t_command *cmd)
{
	if (ft_strncmp(cmd->command, "echo", 5) == 0)
		return (echo(cmd));
	if (ft_strncmp(cmd->command, "exit", 5) == 0)
		return (ft_exit(shell));
	return (CMD_NOT_FOUND);
} */

// Executes a system or local binary using execve
int	execute_sys_n_local_bin(t_data *shell, t_command *cmd)
{
	if (!cmd->command || cmd->command[0] == '\0' || !cmd->path)
		return (CMD_NOT_FOUND);
	execve(cmd->path, cmd->args, shell->env);
	perror("execve");
	return (EXIT_FAILURE);
}

// Executes the given command in a child process
int	execute_cmd(t_data *shell, t_command *cmd)
{
	int	ret;

	if (!cmd->command)
		return (EXIT_FAILURE);
	if (cmd->error != NULL)
	{
		ft_putendl_fd(cmd->error, STDERR_FILENO);
		return (errno); // Return the errno value as the error code
	}
	handle_pipes_and_redirections(cmd);
	/* ret = execute_builtin(shell, cmd); */
	if (ret != CMD_NOT_FOUND)
		return (ret);
	return (execute_sys_n_local_bin(shell, cmd));
}

// Main execute function to fork processes and manage pipes
int	execute(t_data *shell)
{
	t_command	*cmd;

	cmd = shell->command;
	if (!shell || !cmd || !piping(shell) || !open_last_red(shell))
		return (EXIT_FAILURE);

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
		cmd = cmd->next;
	}
	return (wait_for_children());
}
