/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vamachad <vamachad@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 20:01:33 by vamachad          #+#    #+#             */
/*   Updated: 2024/10/28 20:09:33 by vamachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Get the PATH variable from the environment
void	free_array(char **arr)
{
	int	i;

	i = -1;
	while (arr[++i])
		free(arr[i]);
	free(arr);
}

static char	*getenv_from_env(char **env, const char *name)
{
	int	i;
	int	len;

	len = ft_strlen(name);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], name, len) == 0 && env[i][len] == '=')
			return (env[i] + len + 1);
		i++;
	}
	return (NULL);
}

// Concatenate strings and free the first
char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*joined_str;

	joined_str = ft_strjoin(s1, s2);
	if (!joined_str)
		return (NULL);
	free(s1);
	return (joined_str);
}

// Fill command path by searching PATH environment variable
void	fill_command_path(t_command *cmd, t_data *shell)
{
	char	**paths;
	char	*test_path;
	char	*path_env;
	int		i;

	i = 0;
	path_env = getenv_from_env(shell->env, "PATH");
	if (!cmd || !cmd->command || !path_env)
		return ;
	paths = ft_split(path_env, ':');
	if (!paths)
		return ;
	while (paths[i])
	{
		test_path = ft_strjoin_free(ft_strjoin(paths[i], "/"), cmd->command);
		if (access(test_path, F_OK) == 0)
		{
			cmd->path = test_path;
			break ;
		}
		free(test_path);
		i++;
	}
	free_array(paths);
}
