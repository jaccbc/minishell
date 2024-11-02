/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 19:51:51 by joandre-          #+#    #+#             */
/*   Updated: 2024/11/02 02:23:52 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_exit(t_data *shell)
{
	int	i;

	if (shell == NULL)
		exit(EXIT_FAILURE);
	if (shell->env)
	{
		i = 0;
		while (shell->env[i])
			free(shell->env[i++]);
		free(shell->env);
	}
	lstdel_command(shell->command);
	free(shell->user_input);
	rl_clear_history();
	exit(EXIT_SUCCESS);
}
