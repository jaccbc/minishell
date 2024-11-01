/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 22:26:37 by joandre-          #+#    #+#             */
/*   Updated: 2024/11/01 01:38:33 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	init_env(t_data *shell, char **env)
{
	int	i;

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
	return (true);
}

//inicializa a leitura da string do user_input
static bool	init_shell(t_data *shell, char *user_input)
{
	shell->lst = tokenize(user_input);
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
	print_command_list(shell->command);
	echo(shell->command);
	return (lstdel_command(shell->command), true);
}

//será parte do modo interactivo, o loop que pede o input do utilizador
//que será mais tarde lexado, parsado e executado
int	main(int argc, char **argv, char **env)
{
	t_data	shell;

	if (!(argc == 3 || argc == 1))
		return (-1);
	if (init_env(&shell, env) == false)
		return (1);
	if (argc == 3)
		if (ft_strncmp(argv[1], "-c", ft_strlen(argv[1])) == 0)
			if (init_shell(&shell, argv[2]))
				return (0);
	while (argc == 1)
	{
		shell.user_input = readline(PROMPT);
		init_shell(&shell, shell.user_input);
		if (*shell.user_input)
			add_history(shell.user_input);
		free(shell.user_input);
	}
	return (1);
}
