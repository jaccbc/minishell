/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 22:26:37 by joandre-          #+#    #+#             */
/*   Updated: 2024/12/11 23:00:10 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			g_last_exit_code;

static bool	init_env(t_data *shell, char **env)
{
	int	i;

	g_last_exit_code = 0;
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
	return (true);
}

// inicializa a leitura da string do user_input
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
		return (false);
	}
	g_last_exit_code = execute(shell);
	return (lstdel_command(shell->command), true);
}

// será parte do modo interactivo, o loop que pede o input do utilizador
// que será mais tarde lexado, parsado e executado
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
		sighandler_noninteractive();
		if (shell.user_input == NULL)
		{
			exit_cleanup(&shell);
			ft_putendl_fd("exit", STDOUT_FILENO);
			exit(EXIT_SUCCESS);
		}
		init_prompt(&shell, shell.user_input);
		shell.lst = NULL;
		shell.command = NULL;
	}
	if (argc == 3 && ft_strncmp(argv[1], "-c", ft_strlen(argv[1])) == 0)
		if (init_prompt(&shell, argv[2]))
			return (0);
	return (1);
}
