/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 03:17:40 by joandre-          #+#    #+#             */
/*   Updated: 2024/12/11 04:51:54 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	cd_errmsg(char *msg, int err)
{
	if (err <= 2)
		ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
	if (err == 0)
		ft_putendl_fd(msg, STDERR_FILENO);
	else if (err == 1)
	{
		ft_putstr_fd("-", STDERR_FILENO);
		ft_putchar_fd(*msg, STDERR_FILENO);
		ft_putendl_fd(": invalid option", STDERR_FILENO);
	}
	else if (err == 2)
	{
		ft_putstr_fd(msg, STDERR_FILENO);
		ft_putendl_fd(": Not a directory", STDERR_FILENO);
	}
}

// creates environment variable if non existent
char	**create_envar(char *cwd, const char *var, char **env, int size)
{
	char	*new_var;
	char	**new_env;

	new_env = ft_realloc(env, size + 2);
	if (new_env == NULL)
		return (NULL);
	new_var = ft_strjoin(var, cwd);
	if (new_var == NULL)
		return (free_env(new_env), NULL);
	new_env[size] = new_var;
	new_env[++size] = NULL;
	return (new_env);
}

bool	env_change(t_data *shell, const char *var, char *path, char *cwd)
{
	int	i;

	i = 0;
	while (shell->env[i])
		++i;
	shell->env = create_envar(cwd, var, shell->env, i);
	if (cwd != path)
		free(cwd);
	if (shell->env == NULL)
		return (perror("minishell"), false);
	return (true);
}

bool	update_env(char *path, const char *var, t_data *shell)
{
	int		i;
	char	*cwd;
	char	*new_var;

	if (path)
		cwd = path;
	else
		cwd = getcwd(NULL, 0);
	if (cwd == NULL)
		return (perror("minishell"), false);
	i = 0;
	while (shell->env[i] && ft_strncmp(shell->env[i], var, ft_strlen(var)))
		++i;
	if (shell->env[i] == NULL)
		return (env_change(shell, var, path, cwd));
	new_var = ft_strjoin(var, cwd);
	if (cwd != path)
		free(cwd);
	if (new_var == NULL)
		return (perror("minishell"), false);
	free(shell->env[i]);
	shell->env[i] = new_var;
	return (true);
}
