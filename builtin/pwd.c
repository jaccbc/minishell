/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vamachad <vamachad@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 13:06:03 by vamachad          #+#    #+#             */
/*   Updated: 2024/12/08 02:49:58 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_pwd(t_data *shell)
{
	char	buf[4096];
	char	*err;
	char	*cwd;
	int		i;

	i = -1;
	if (!shell->env)
		return (EXIT_FAILURE);
	while (shell->env[++i])
	{
		if (ft_strncmp(shell->env[i], "PWD", 3) == 0)
			return (ft_putendl_fd((shell->env[i] + 4), STDOUT_FILENO),
				EXIT_SUCCESS);
	}
	cwd = getcwd(buf, 4096);
	if (cwd)
	{
		ft_putendl_fd(cwd, STDOUT_FILENO);
		return (EXIT_SUCCESS);
	}
	err = mini_errmsg("pwd", NULL, strerror(errno), true);
	ft_putendl_fd(err, STDERR_FILENO);
	free(err);
	return (EXIT_FAILURE);
}
