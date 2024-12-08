/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 03:17:40 by joandre-          #+#    #+#             */
/*   Updated: 2024/12/08 03:18:39 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	cd_errmsg(char *msg, int err)
{
	if (err <= 2)
		ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
	if (err == 0)
		ft_putendl_fd(msg, STDERR_FILENO);
	else if (err == 1)
	{
		ft_putstr_fd("-", STDERR_FILENO);
		ft_putchar_fd(*msg, STDERR_FILENO);
		ft_putendl_fd(": invalid option", STDERR_FILENO);
	}
	else if (err == 2)
	{
		ft_putstr_fd(msg, STDERR_FILENO);
		ft_putendl_fd(": Not a directory", STDERR_FILENO);
	}
}
