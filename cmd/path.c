/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vamachad <vamachad@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 20:01:33 by vamachad          #+#    #+#             */
/*   Updated: 2024/10/31 15:46:57 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Get the PATH variable from the environment
static void	free_splits(char **path_split)
{
	int	i;

	i = -1;
	while (path_split[++i])
		free(path_split[i]);
	free(path_split);
}

static char	*getenv_path(char **env, const char *var)
{
	int	i;
	int	len;

	len = ft_strlen(var);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], var, len) == 0 && env[i][len] == '=')
			return (env[i] + len + 1);
		i++;
	}
	return (NULL);
}

// Concatenate strings and free the first
static char	*getfull_path(char *path, char *file)
{
	char	*full_path;

	if (path == NULL)
		return (NULL);
	full_path = ft_strjoin(path, file);
	free(path);
	if (full_path == NULL)
		return (NULL);
	return (full_path);
}

// Fill command path by searching PATH environment variable
void	fill_command_path(t_command *cmd, t_data *shell)
{
	char	**path_split;
	char	*full_path;
	char	*path_env;
	int		i;

	if (!cmd || !cmd->command)
		return ;
	path_env = getenv_path(shell->env, "PATH");
	if (!path_env)
		return ;
	path_split = ft_split(path_env, ':');
	if (!path_split)
		return ;
	i = -1;
	while (path_split[++i])
	{
		full_path = getfull_path(ft_strjoin(path_split[i], "/"), cmd->command);
		if (access(full_path, F_OK) == 0)
		{
			cmd->path = full_path;
			break ;
		}
		free(full_path);
	}
	free_splits(path_split);
}
