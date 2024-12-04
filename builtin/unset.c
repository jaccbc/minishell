/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vamachad <vamachad@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 13:03:02 by vamachad          #+#    #+#             */
/*   Updated: 2024/12/04 13:03:06 by vamachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	free_array(char **arr)
{
	int	i;

	if (!arr)
		return (EXIT_SUCCESS);
	i = -1;
	while (arr[++i])
		free(arr[i]);
	free(arr);
	return (EXIT_SUCCESS);
}

static int	fill_new_env(char **new_env, char **old_env, int var_index)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (old_env[i])
	{
		if (i == var_index)
		{
			i++;
			continue ;
		}
		new_env[j] = ft_strdup(old_env[i]);
		if (!new_env[j])
		{
			free_array(new_env);
			return (EXIT_FAILURE);
		}
		j++;
		i++;
	}
	new_env[j] = NULL;
	return (EXIT_SUCCESS);
}

int	redo_env(t_data *shell, int var_index)
{
	int		env_size;
	char	**new_env;

	env_size = 0;
	while (shell->env[env_size])
		env_size++;
	new_env = ft_calloc(env_size, sizeof(char *));
	if (!new_env)
		return (EXIT_FAILURE);
	if (fill_new_env(new_env, shell->env, var_index) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	free_array(shell->env);
	shell->env = new_env;
	return (EXIT_SUCCESS);
}

int	ft_unset(t_data *shell)
{
	int	i;
	int	j;
	int	var_len;

	if (!shell || !shell->env || !shell->command || !shell->command->args[1])
		return (EXIT_FAILURE);
	j = 0;
	while (shell->command->args[++j])
	{
		i = 0;
		while (shell->env[i])
		{
			var_len = ft_strlen(shell->command->args[j]);
			if (!ft_strncmp(shell->command->args[j], shell->env[i], var_len)
				&& shell->env[i][var_len] == '=')
			{
				if (redo_env(shell, i) == EXIT_FAILURE)
					return (EXIT_FAILURE);
				break ;
			}
			i++;
		}
	}
	return (EXIT_SUCCESS);
}
