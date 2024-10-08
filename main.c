/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 22:26:37 by joandre-          #+#    #+#             */
/*   Updated: 2024/10/08 02:24:39 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//será parte do modo interactivo, o loop que pede o input do utilizador
//que será mais tarde lexado, parsado e executado
int	main(int argc, char **argv)
{
	t_data	shell;
	char	*user_input;

	if (argc == 3)
	{
		if (ft_strncmp(argv[1], "-c", 2) == 0)
		{
			shell.lst = tokenize(argv[2]);
			print_list(shell.lst);
			free_token(shell.lst);
		}
		return (0);
	}
	while (argc == 1)
	{
		user_input = readline(PROMPT);
		shell.lst = tokenize(user_input);
		if (!shell.lst)
			return (0);
		add_history(user_input);
		free(user_input);
		print_list(shell.lst);
		free_token(shell.lst);
	}
	return (1);
}
