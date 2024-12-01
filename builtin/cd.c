/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 21:36:54 by joandre-          #+#    #+#             */
/*   Updated: 2024/12/01 03:39:21 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	cd_errmsg(char *msg, int err)
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

static bool	check_error(t_command *cmd, int argc, char **env, char **path)
{
	struct stat	data;

	if ((argc == 1) || (argc == 2
			&& (ft_strcmp(cmd->args[1], "--") || ft_strcmp(cmd->args[1], "-"))))
		return (false);
	if (argc > 3 || (argc >= 3 && !ft_strcmp(cmd->args[1], "--")))
		return (cd_errmsg("too many arguments", 0), true);
	if (getenv_path(env, "OLDPWD") == NULL)
		if ((argc == 2 && ft_strcmp(cmd->args[1], "-"))
			|| (argc == 3 && ft_strcmp(cmd->args[1], "--")
				&& ft_strcmp(cmd->args[2], "-")))
			return (cd_errmsg("OLDPWD not set", 0), true);
	if (argc == 3 && ft_strcmp(cmd->args[2], "-"))
		return (false);
	if (argc == 2 && cmd->args[1][0] == '-' && !ft_strcmp(cmd->args[1], "--"))
		return (cd_errmsg(cmd->args[1] + 1, 1), true);
	(*path) = expand_path(env, cmd->args[argc - 1]);
	if (*path == NULL)
		return (cd_errmsg(strerror(errno), 0), true);
	if (stat(*path, &data))
		return (free(*path), cd_errmsg(strerror(errno), 0), true);
	if (S_ISDIR(data.st_mode) == false)
		return (cd_errmsg(*path, 2), free(*path), true);
	return (false);
}

static bool	set_oldpwd(char **env)
{
	int		i;

	if (getenv_path(env, "OLDPWD"))
		return (true);
	i = 0;
	while (env[i])
		++i;
	env = ft_realloc(env, i + 2);
	if (env == NULL)
		return (ft_putendl_fd(strerror(errno), STDERR_FILENO),
			false);
	env[i] = ft_strjoin("OLDPWD=", getenv_path(env, "PWD"));
	if (env[i++] == NULL)
		return (ft_putendl_fd(strerror(errno), STDERR_FILENO),
			false);
	env[i] = NULL;
	return (true);
}

static bool	update_env(char *path, const char *var, char **env)
{
	int		i;
	char	*cwd;

	i = 0;
	while (env[i] && ft_strncmp(env[i], var, ft_strlen(var)))
		++i;
	if (path)
		cwd = path;
	else
		cwd = getcwd(NULL, 0);
	if (cwd == NULL)
		return (cd_errmsg(strerror(errno), 0), false);
	free(env[i]);
	env[i] = ft_strjoin(var, cwd);
	if (env[i] == NULL)
		return (free(cwd), cd_errmsg(strerror(errno), 0), false);
	return (free(cwd), true);
}

// code 0 = cd || cd --
// code 1 = cd - || cd -- -
static int	switch_dir(char *path, char **env, int code)
{
	char	*old;

	old = getcwd(NULL, 0);
	if (old == NULL)
		return (free(path), EXIT_FAILURE);
	if (code == 0 && !path)
		path = ft_strdup(getenv_path(env, "HOME"));
	else if (code == 1 && !path)
		path = ft_strdup(getenv_path(env, "OLDPWD"));
	if (path == NULL)
		return (free(old), cd_errmsg(strerror(errno), 0), EXIT_FAILURE);
	if (code == 1)
		ft_putendl_fd(path, STDOUT_FILENO);
	if (chdir(path))
		return (free(path), free(old), cd_errmsg(strerror(errno), 0),
			EXIT_FAILURE);
	if (update_env(old, "OLDPWD=", env) && update_env(NULL, "PWD=", env))
		return (free(path), EXIT_SUCCESS);
	return (free(path), EXIT_SUCCESS);
}

int	ft_cd(t_command *cmd, char **env)
{
	int		argc;
	char	*path;

	if (!cmd || !cmd->command || !cmd->args || !env)
		return (EXIT_FAILURE);
	argc = 0;
	while (cmd->args[argc])
		++argc;
	path = NULL;
	if (check_error(cmd, argc, env, &path) || !set_oldpwd(env))
		return (EXIT_FAILURE);
	if (argc == 1 || (argc == 2 && ft_strcmp(cmd->args[1], "--")))
		return (switch_dir(path, env, 0));
	if (argc == 2 && ft_strcmp(cmd->args[1], "-"))
		return (switch_dir(path, env, 1));
	if (argc == 3
		&& ft_strcmp(cmd->args[1], "--") && ft_strcmp(cmd->args[2], "-"))
		return (switch_dir(path, env, 1));
	return (switch_dir(path, env, 2));
}
