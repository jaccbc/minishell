/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vamachad <vamachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 22:26:37 by joandre-          #+#    #+#             */
/*   Updated: 2025/01/14 00:37:53 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	init_env(t_data *shell, char **env)
{
	int	i;

	shell->status = 0;
	i = 0;
	while (env[i])
		++i;
	shell->env = ft_calloc(i + 1, sizeof(char *));
	if (shell->env == NULL)
		return (false);
	i = -1;
	while (env[++i])
	{
		shell->env[i] = ft_strdup(env[i]);
		if (!shell->env[i])
		{
			while (i)
				free(shell->env[i--]);
			return (free(shell->env), false);
		}
	}
	shell->env[i] = NULL;
	shell->home_dir = getenv_path(env, "HOME");
	shell->pwd_backup = getcwd(NULL, 0);
	return (true);
}

// initiailizes the read of the user's prompt
static bool	init_prompt(t_data *shell, char *user_input)
{
	shell->lst = tokenize(user_input);
	if (*user_input)
		add_history(user_input);
	free(user_input);
	if (shell->lst == NULL)
		return (false);
	if (check_syntax(shell) == false)
		return (lstdel_token(shell->lst), false);
	if (!final_parse(shell))
	{
		lstdel_token(shell->lst);
		lstdel_command(shell->command);
		shell->lst = NULL;
		shell->command = NULL;
		return (false);
	}
	g_signal = 0;
	shell->status = execute(shell);
	lstdel_command(shell->command);
	shell->lst = NULL;
	shell->command = NULL;
	return (true);
}

int	main(int argc, char **argv, char **env)
{
	t_data	shell;

	if (!(argc == 3 || argc == 1))
		return (-1);
	if (init_env(ft_memset(&shell, 0, sizeof(shell)), env) == false)
		return (1);
	while (argc == 1)
	{
		sighandler();
		shell.user_input = readline(PROMPT);
		if (g_signal == SIGINT)
			shell.status = 130;
		sighandler_noninteractive();
		if (shell.user_input == NULL)
		{
			exit_cleanup(&shell);
			ft_putendl_fd("exit", STDOUT_FILENO);
			exit(EXIT_SUCCESS);
		}
		init_prompt(&shell, shell.user_input);
	}
	if (argc == 3 && ft_strncmp(argv[1], "-c", ft_strlen(argv[1])) == 0)
		if (init_prompt(&shell, argv[2]))
			return (0);
	return (free(shell.pwd_backup), 1);
}
