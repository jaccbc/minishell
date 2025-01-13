/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vamachad <vamachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 20:09:57 by vamachad          #+#    #+#             */
/*   Updated: 2025/01/13 20:25:00 by vamachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	update_pwd_backup(t_data *shell)
{
	if (getenv_path(shell->env, "PWD"))
	{
		free(shell->pwd_backup);
		shell->pwd_backup = ft_strdup(getenv_path(shell->env, "PWD"));
	}
}

void	cleanup_shell(t_data *shell)
{
	free(shell->pwd_backup);
	lstdel_command(shell->command);
	free_env(shell->env);
}
