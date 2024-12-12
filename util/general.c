/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   general.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 01:57:01 by joandre-          #+#    #+#             */
/*   Updated: 2024/12/12 00:47:50 by vamachad         ###   ########.fr       */
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

char	*expand_path(t_data *shell, char *str)
{
	char	*pwd;
	char	*path;

	pwd = getcwd(NULL, 0);
	if (pwd == NULL)
		return (perror("minishell"), NULL);
	if (ft_strncmp(str, "./", 2) == 0)
		path = ft_strjoin(pwd, str + 1);
	else if (ft_strncmp(str, "~/", 2) == 0 && getenv_path(shell->env,
			"HOME") != NULL)
		path = ft_strjoin(getenv_path(shell->env, "HOME"), str + 1);
	else if (ft_strncmp(str, "~/", 2) == 0 && getenv_path(shell->env,
			"HOME") == NULL)
		path = ft_strjoin(shell->home_dir, str + 1);
	else if (ft_strncmp(str, "../", 3) == 0 || ft_strncmp(str, "..", 3) == 0)
		path = expand_backpath(str, pwd, ft_strlen(pwd));
	else
		path = ft_strdup(str);
	if (path == NULL)
		return (free(pwd), perror("minishell"), NULL);
	return (free(pwd), path);
}
