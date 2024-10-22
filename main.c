/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 22:26:37 by joandre-          #+#    #+#             */
/*   Updated: 2024/10/22 03:32:21 by joandre-         ###   ########.fr       */
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
	i = 0;
	while (env[i])
	{
		shell->env[i] = ft_strdup(env[i]);
		if (!shell->env[i++])
			return (false);
	}
	return (true);
}

//inicializa a leitura da string do user_input
static bool	init_shell(t_data *shell, char *user_input, char **env)
{
	if (init_env(shell, env) == false)
		return (false);
	shell->lst = tokenize(user_input);
	if (shell->lst == NULL)
		return (false);
	add_history(user_input);
	free(user_input);
	if (check_syntax(shell) == false)
		return (lstdel_token(shell->lst), false);
	print_list(shell->lst);
	echo(shell->lst);
	lstdel_token(shell->lst);
	return (true);
}

//será parte do modo interactivo, o loop que pede o input do utilizador
//que será mais tarde lexado, parsado e executado
int	main(int argc, char **argv, char **env)
{
	t_data	shell;
	char	*user_input;

	if (argc == 3)
		if (ft_strncmp(argv[1], "-c", ft_strlen(argv[1])) == 0)
			if (init_shell(&shell, argv[2], env))
				return (0);
	while (argc == 1)
	{
		user_input = readline(PROMPT);
		init_shell(&shell, user_input, env);
	}
	return (1);
}
