/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 19:51:51 by joandre-          #+#    #+#             */
/*   Updated: 2024/11/01 02:55:34 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static bool	verify_command(t_command *cmd, bool *nl)
{
	if (!cmd || !cmd->command || !cmd->args)
		return (false);
	if (ft_strncmp(cmd->command, "echo", ft_strlen(cmd->command)))
		return (false);
	if (cmd->args[0] && cmd->args[1])
		if (ft_strncmp(cmd->args[1], "-n", ft_strlen(cmd->args[1])) == 0)
			*nl = false;
	return (true);
}

int	echo(t_command *cmd)
{
	bool	nl;
	int		i;

	nl = true;
	if (verify_command(cmd, &nl) == false)
		return (EXIT_FAILURE);
	i = 1;
	if (nl == false)
		++i;
	while (cmd->args[i])
	{
		ft_putstr_fd(cmd->args[i], STDOUT_FILENO);
		if (cmd->args[++i])
			ft_putstr_fd(" ", STDOUT_FILENO);
	}
	if (nl)
		ft_putstr_fd("\n", STDOUT_FILENO);
	return (EXIT_SUCCESS);
}
