/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 22:26:37 by joandre-          #+#    #+#             */
/*   Updated: 2024/10/06 02:23:20 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//será parte do modo interactivo, o loop que pede o input do utilizador
//que será mais tarde lexado, parsado e executado
int	main(void)
{
	t_data	shell;
	char	*user_input;

	while (1)
	{
		user_input = readline(PROMPT);
		shell.lst = tokenize(user_input);
		add_history(user_input);
		print_list(shell.lst);
		free_tokens(shell.lst);
		free(user_input);
	}
}
