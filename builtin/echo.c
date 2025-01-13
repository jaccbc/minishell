/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vamachad <vamachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 19:51:51 by joandre-          #+#    #+#             */
/*   Updated: 2025/01/13 17:01:56 by vamachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static bool	is_valid_n_flag(char *arg)
{
	int	j;

	j = 1;
	if (arg[0] != '-' || arg[1] != 'n')
		return (false);
	while (arg[j] == 'n')
		j++;
	if (arg[j] != '\0')
		return (false);
	return (true);
}

static bool	verify_command(t_command *cmd, bool *nl)
{
	int	i;

	if (!cmd || !cmd->command || !cmd->args)
		return (false);
	if (ft_strncmp(cmd->command, "echo", ft_strlen(cmd->command)) != 0)
		return (false);
	*nl = true;
	i = 1;
	while (cmd->args[i] && is_valid_n_flag(cmd->args[i]) == true)
	{
		*nl = false;
		i++;
	}
	return (true);
}

static int	skip_n_flags(t_command *cmd)
{
	int	i;

	i = 1;
	while (cmd->args[i] && is_valid_n_flag(cmd->args[i]) == true)
		i++;
	return (i);
}

int	ft_echo(t_command *cmd)
{
	bool	nl;
	int		i;

	if (verify_command(cmd, &nl) == false)
		return (EXIT_FAILURE);
	i = skip_n_flags(cmd);
	while (cmd->args[i])
	{
		ft_putstr_fd(cmd->args[i], STDOUT_FILENO);
		i++;
		if (cmd->args[i])
			ft_putstr_fd(" ", STDOUT_FILENO);
	}
	if (nl == true)
		ft_putstr_fd("\n", STDOUT_FILENO);
	return (EXIT_SUCCESS);
}
