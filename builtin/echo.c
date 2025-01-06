/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vamachad <vamachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 19:51:51 by joandre-          #+#    #+#             */
/*   Updated: 2025/01/06 13:38:56 by vamachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static bool	verify_command(t_command *cmd, bool *nl)
{
	int	i;

	if (!cmd || !cmd->command || !cmd->args)
		return (false);
	if (ft_strncmp(cmd->command, "echo", ft_strlen(cmd->command)))
		return (false);
	if (cmd->args[0] && cmd->args[1])
	{
		i = 0;
		if (cmd->args[1][0] == '-' && cmd->args[1][1] == 'n')
			while (cmd->args[1][++i] == 'n')
				*nl = false;
		if (cmd->args[1][i] != '\0')
			*nl = true;
	}
	return (true);
}

int	ft_echo(t_command *cmd)
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
		if (i == 1 && ft_strncmp(cmd->args[1], "-", 2) == 0)
		{
			i++;
			continue;
		}
		ft_putstr_fd(cmd->args[i], STDOUT_FILENO);
		if (cmd->args[++i])
			ft_putstr_fd(" ", STDOUT_FILENO);
	}
	if (nl)
		ft_putstr_fd("\n", STDOUT_FILENO);
	return (EXIT_SUCCESS);
}
