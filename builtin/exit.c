/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 19:51:51 by joandre-          #+#    #+#             */
/*   Updated: 2024/12/19 21:11:17 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

long	ft_atol(char *str_nb)
{
	int					i;
	unsigned long long	nb;
	int					neg;

	neg = 1;
	i = 0;
	while (ft_isspace(str_nb[i]))
		i++;
	if (str_nb[i] == '+')
		i++;
	else if (str_nb[i] == '-')
	{
		neg = -1;
		i++;
	}
	nb = 0;
	while (str_nb[i])
	{
		nb = nb * 10 + (str_nb[i] - '0');
		if ((nb > LONG_MAX && neg == 1)
			|| (nb > -(unsigned long)LONG_MIN && neg == -1))
			g_last_exit_code = 2;
		i++;
	}
	return (nb * neg);
}

static bool	check_args(t_command *cmd)
{
	int	i;

	if (!cmd)
		g_last_exit_code = EXIT_FAILURE;
	i = -1;
	if (cmd->args)
	{
		if (cmd->args[2])
		{
			g_last_exit_code = EXIT_FAILURE;
			return (false);
		}
		while (cmd->args[1][++i])
			if (!ft_isdigit(cmd->args[1][i]) && !ft_isspace(cmd->args[1][i])
			&& cmd->args[1][i] != '+' && cmd->args[1][i] != '-')
				g_last_exit_code = 2;
	}
	else
		g_last_exit_code = EXIT_SUCCESS;
	return (true);
}

void	prepare_exit(char *arg_1)
{
	char	*err;
	long	nb;

	nb = ft_atol(arg_1);
	if (g_last_exit_code == 2)
	{
		err = mini_errmsg("exit", arg_1, "numeric argument required", true);
		ft_putendl_fd(err, STDERR_FILENO);
		free(err);
		return ;
	}
	g_last_exit_code = nb % 256;
}

void	exit_cleanup(t_data *shell)
{
	int		i;

	if (shell->env)
	{
		i = 0;
		while (shell->env[i])
			free(shell->env[i++]);
		free(shell->env);
	}
	if (shell->command)
		lstdel_command(shell->command);
	rl_clear_history();
}

int	ft_exit(t_data *shell, t_command *cmd)
{
	char	*err;

	if (cmd == NULL)
		exit(EXIT_FAILURE);
	if (!cmd->prev && !cmd->next)
		ft_putendl_fd("exit", 1);
	if (!cmd->args[1])
	{
		exit_cleanup(shell);
		exit(EXIT_SUCCESS);
	}
	if (!check_args(cmd))
	{
		err = mini_errmsg("exit", NULL, "too many arguments", true);
		ft_putendl_fd(err, STDERR_FILENO);
		free (err);
		return (g_last_exit_code);
	}
	prepare_exit(cmd->args[1]);
	exit_cleanup(shell);
	exit(g_last_exit_code);
}
