/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 22:26:37 by joandre-          #+#    #+#             */
/*   Updated: 2024/10/05 23:37:48 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//será parte do modo interactivo, o loop que pede o input do utilizador
//que será mais tarde lexado, parsado e executado
int	main(void)
{
	char	*user_input;

	while (1)
	{
		user_input = readline(PROMPT);
		tokenize(user_input);
		add_history(user_input);
		printf("%s\n", user_input);
		free(user_input);
	}
}
