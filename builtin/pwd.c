/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vamachad <vamachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 13:06:03 by vamachad          #+#    #+#             */
/*   Updated: 2025/01/13 20:38:17 by vamachad         ###   ########.fr       */
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
	if (cwd == NULL)
	{
		cwd = ft_strdup(shell->pwd_backup);
		if (cwd == NULL)
			return (perror("minishell"), EXIT_FAILURE);
	}
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
