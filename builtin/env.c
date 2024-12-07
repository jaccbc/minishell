/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vamachad <vamachad@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 13:10:58 by vamachad          #+#    #+#             */
/*   Updated: 2024/12/04 13:10:59 by vamachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_env(t_data *shell, t_command *cmd)
{
	int		i;
	char	*err;

	if (cmd->args && cmd->args[1])
	{
		err = mini_errmsg("env", NULL, "too many arguments", true);
		ft_putendl_fd(err, STDERR_FILENO);
		free(err);
		return (EXIT_FAILURE);
	}
	if (!shell || !shell->env)
		return (EXIT_FAILURE);
	i = -1;
	while (shell->env[++i])
		ft_putendl_fd(shell->env[i], STDOUT_FILENO);
	return (EXIT_SUCCESS);
}
