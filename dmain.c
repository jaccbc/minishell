/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dmain.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 22:26:37 by joandre-          #+#    #+#             */
/*   Updated: 2024/10/18 02:40:04 by vamachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	init_env(t_data *shell, char **env)
{
	int	i;

	i = 0;
	while (env[i])
		i++;
	shell->env = ft_calloc(i + 1, sizeof * shell->env);
	if (!shell->env)
		return (false);
	i = 0;
	while (env[i])
	{
		shell->env[i] = ft_strdup(env[i]);
		if (!shell->env[i])
			return (false);
		i++;
	}
	return (true);
}

// será parte do modo interactivo, o loop que pede o input do utilizador
// que será mais tarde lexado, parsado e executado
int	main(int argc, char **argv, char **env)
{
	t_data	shell;
	char	*user_input;

	ft_bzero(&shell, sizeof(t_data));
	init_env(&shell, env);
	if (argc == 3)
	{
		if (ft_strncmp(argv[1], "-c", ft_strlen(argv[1])))
			return (1);
		shell.lst = tokenize(argv[2]);
		if (!check_syntax(&shell))
			return (1);
		print_list(shell.lst);
		echo(shell.lst);
		free_token(shell.lst);
	}
	while (argc == 1)
	{
		user_input = readline(PROMPT);
		shell.lst = tokenize(user_input);
		add_history(user_input);
		free(user_input);
		if (!check_syntax(&shell))
			continue ;
		printf("***MAIN***\n");
		print_list(shell.lst);
		/* echo(shell.lst); */
		free_token(shell.lst);
	}
	return (0);
}
