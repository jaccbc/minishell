/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vamachad <vamachad@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 13:05:20 by vamachad          #+#    #+#             */
/*   Updated: 2025/01/10 18:29:04 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	key_is_valid(char *arg)
{
	int		i;
	bool	status;
	char	*err;

	status = true;
	if (!ft_isalpha(arg[0]) && arg[0] != '_')
	{
		err = mini_errmsg("export", arg, "not a valid identifier", true);
		ft_putendl_fd(err, STDERR_FILENO);
		free(err);
		status = false;
	}
	i = 1;
	while (arg[i] && arg[i] != '=')
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
		{
			err = mini_errmsg("export", arg, "not a valid identifier", true);
			ft_putendl_fd(err, STDERR_FILENO);
			free(err);
			status = false;
		}
		i++;
	}
	return (status);
}

int	get_var_index(t_data *shell, char *arg)
{
	int	i;
	int	key_len;

	key_len = ft_strchr(arg, '=') - arg;
	i = -1;
	while (shell->env[++i])
	{
		if (!ft_strncmp(shell->env[i], arg, key_len)
			&& shell->env[i][key_len] == '=')
			return (i);
	}
	return (-1);
}

int	replace_var_value(t_data *shell, char *var_n_value, int index)
{
	free(shell->env[index]);
	shell->env[index] = ft_strdup(var_n_value);
	if (!shell->env[index])
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	add_var(t_data *shell, char *var_n_value)
{
	char	**new_env;
	int		env_count;

	env_count = 0;
	while (shell->env[env_count])
		env_count++;
	new_env = ft_realloc(shell->env, sizeof(char *) * (env_count + 2));
	if (!new_env)
		return (EXIT_FAILURE);
	new_env[env_count] = ft_strdup(var_n_value);
	if (!new_env[env_count])
		return (EXIT_FAILURE);
	new_env[env_count + 1] = NULL;
	shell->env = new_env;
	return (EXIT_SUCCESS);
}

int	ft_export(t_data *shell, t_command *cmd)
{
	int		i;
	int		status;

	i = -1;
	status = EXIT_SUCCESS;
	while (cmd->args[++i])
	{
		if (!key_is_valid(cmd->args[i]))
			status = EXIT_FAILURE;
		else if (ft_strchr(cmd->args[i], '=') != NULL)
		{
			if (get_var_index(shell, cmd->args[i]) != -1)
			{
				if (replace_var_value(shell, cmd->args[i],
						get_var_index(shell, cmd->args[i])) == EXIT_FAILURE)
					status = EXIT_FAILURE;
			}
			else if (add_var(shell, cmd->args[i]) == EXIT_FAILURE)
				status = EXIT_FAILURE;
		}
	}
	if (i == 1)
		return (export_declare(shell), EXIT_SUCCESS);
	return (status);
}
