/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   general.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 01:57:01 by joandre-          #+#    #+#             */
/*   Updated: 2024/12/08 02:08:28 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	is_builtin(t_command *cmd)
{
	if (!cmd->command)
		return (false);
	if (ft_strncmp(cmd->command, "echo", 5) == 0)
		return (true);
	if (ft_strncmp(cmd->command, "exit", 5) == 0)
		return (true);
	if (ft_strncmp(cmd->command, "env", 4) == 0)
		return (true);
	if (ft_strncmp(cmd->command, "pwd", 4) == 0)
		return (true);
	if (ft_strncmp(cmd->command, "unset", 6) == 0)
		return (true);
	if (ft_strncmp(cmd->command, "export", 7) == 0)
		return (true);
	if (ft_strncmp(cmd->command, "cd", 3) == 0)
		return (true);
	return (false);
}

char	**ft_realloc(char **array, size_t new_size)
{
	char	**new_array;
	size_t	i;

	new_array = malloc(sizeof(char *) * new_size);
	if (!new_array)
		return (NULL);
	i = 0;
	while (array && array[i] && i < new_size - 1)
	{
		new_array[i] = array[i];
		i++;
	}
	while (i < new_size - 1)
		new_array[i++] = NULL;
	free(array);
	return (new_array);
}

void	free_env(char **env)
{
	int	i;

	if (!env)
		return ;
	i = -1;
	while (env[++i])
		free(env[i]);
	free(env);
}

static char	*expand_backpath(char *str, char *pwd, int i)
{
	if (ft_strncmp(str, "../", 3) == 0)
	{
		while (ft_strncmp(str, "../", 3) == 0)
		{
			str += 3;
			while (i)
				if (pwd[--i] == '/')
					break ;
		}
		pwd[i + 1] = '\0';
		return (ft_strjoin(pwd, str));
	}
	while (i)
	{
		if (pwd[--i] == '/')
		{
			if (i == 0)
				pwd[i + 1] = '\0';
			else
				pwd[i] = '\0';
			break ;
		}
	}
	return (ft_strdup(pwd));
}

char	*expand_path(char **env, char *str)
{
	char	*pwd;

	pwd = getenv_path(env, "PWD");
	if (pwd == NULL)
		return (ft_putendl_fd("PWD not set", STDERR_FILENO), NULL);
	if (ft_strncmp(str, "./", 2) == 0)
		return (ft_strjoin(getenv_path(env, "PWD"), str + 1));
	if (ft_strncmp(str, "~/", 2) == 0)
		return (ft_strjoin(getenv_path(env, "HOME"), str + 1));
	if (ft_strncmp(str, "../", 3) == 0)
		return (expand_backpath(str, pwd, ft_strlen(pwd)));
	if (ft_strncmp(str, "..", 3) == 0)
		return (expand_backpath(str, pwd, ft_strlen(pwd)));
	return (ft_strdup(str));
}
