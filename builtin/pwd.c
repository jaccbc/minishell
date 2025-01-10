/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vamachad <vamachad@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 13:06:03 by vamachad          #+#    #+#             */
/*   Updated: 2025/01/10 17:49:36 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_pwd(t_data *shell)
{
	char	*err;
	char	*cwd;

	if (!shell->env)
		return (EXIT_FAILURE);
	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		ft_putendl_fd(cwd, STDOUT_FILENO);
		return (free(cwd), EXIT_SUCCESS);
	}
	err = mini_errmsg("pwd", NULL, strerror(errno), true);
	ft_putendl_fd(err, STDERR_FILENO);
	free(err);
	return (EXIT_FAILURE);
}
