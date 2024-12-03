#include "../minishell.h"

bool	key_is_valid(char *arg)
{
	int	i;
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
	int	env_count;

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

int	ft_export(t_data *shell)
{
	char	**args;
	int		i;
	int		index;
	int		status;

	i = 0;
	status = EXIT_SUCCESS;
	args = shell->command->args;
	while (args[++i])
	{
		if (!key_is_valid(args[i]))
			status = EXIT_FAILURE;
		else if (ft_strchr(args[i], '=') != NULL)
		{
			index = get_var_index(shell, args[i]);
			if (index != -1)
			{
				if (replace_var_value(shell, args[i], index) == EXIT_FAILURE)
					status = EXIT_FAILURE;
			}
			else if (add_var(shell, args[i]) == EXIT_FAILURE)
				status = EXIT_FAILURE;
		}
	}
	return (status);
}
