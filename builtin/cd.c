/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vamachad <vamachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 21:36:54 by joandre-          #+#    #+#             */
/*   Updated: 2025/01/13 20:13:30 by vamachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static bool	check_error(t_command *cmd, int argc, t_data *shell, char **path)
{
	struct stat	data;

	if (getenv_path(shell->env, "OLDPWD") == NULL)
		if ((argc == 2 && ft_strcmp(cmd->args[1], "-")) || (argc == 3
				&& ft_strcmp(cmd->args[1], "--")
				&& ft_strcmp(cmd->args[2], "-")))
			return (cd_errmsg("OLDPWD not set", 0), true);
	if ((argc == 1) || (argc == 2 && (ft_strcmp(cmd->args[1], "--")
				|| ft_strcmp(cmd->args[1], "-"))))
		return (false);
	if (argc > 3 || (argc >= 3 && !ft_strcmp(cmd->args[1], "--")))
		return (cd_errmsg("too many arguments", 0), true);
	if (argc == 3 && ft_strcmp(cmd->args[2], "-"))
		return (false);
	if (argc == 2 && cmd->args[1][0] == '-' && !ft_strcmp(cmd->args[1], "--"))
		return (cd_errmsg(cmd->args[1] + 1, 1), true);
	(*path) = expand_path(shell, cmd->args[argc - 1]);
	if (*path == NULL)
		return (true);
	if (stat(*path, ft_memset(&data, 0, sizeof(data))))
		return (free(*path), cd_errmsg(strerror(errno), 0), true);
	if (S_ISDIR(data.st_mode) == false)
		return (cd_errmsg(*path, 2), free(*path), true);
	return (false);
}

bool	set_oldpwd(t_data *shell)
{
	int		i;
	char	**new_env;
	char	*pwd;

	if (getenv_path(shell->env, "OLDPWD"))
		return (true);
	i = 0;
	while (shell->env[i])
		++i;
	pwd = getcwd(NULL, 0);
	if (pwd == NULL)
		return (perror("minishell"), false);
	new_env = ft_realloc(shell->env, i + 2);
	if (new_env == NULL)
		return (free(pwd), perror("minishell:"), false);
	new_env[i] = ft_strjoin("OLDPWD=", pwd);
	if (new_env[i++] == NULL)
		return (free_env(new_env), free(pwd), perror("minishell:"), false);
	new_env[i] = NULL;
	shell->env = new_env;
	return (free(pwd), true);
}

// code 0 = cd || cd --
// code 1 = cd - || cd -- -
int	switch_dir(char *path, t_data *shell, int code)
{
	char	*old;

	if (code == 0 && !path && !getenv_path(shell->env, "HOME"))
		return (cd_errmsg("HOME not set", 0), EXIT_FAILURE);
	else if (code == 1 && !path && !getenv_path(shell->env, "OLDPWD"))
		return (cd_errmsg("OLDPWD not set", 0), EXIT_FAILURE);
	old = getcwd(NULL, 0);
	if (old == NULL)
		old = ft_strdup(shell->pwd_backup);
	if (old == NULL)
		return (free(path), perror("minishell"), EXIT_FAILURE);
	if (code == 0 && !path)
		path = ft_strdup(getenv_path(shell->env, "HOME"));
	else if (code == 1 && !path)
		path = ft_strdup(getenv_path(shell->env, "OLDPWD"));
	if (path == NULL)
		return (free(old), perror("minishell"), EXIT_FAILURE);
	if (chdir(path))
		return (free(old), free(path), perror("minishell: cd"), EXIT_FAILURE);
	if (code == 1)
		ft_putendl_fd(path, STDOUT_FILENO);
	if (update_env(old, "OLDPWD=", shell) && update_env(NULL, "PWD=", shell))
		return (free(old), free(path), EXIT_SUCCESS);
	return (free(old), free(path), EXIT_FAILURE);
}

int	ft_cd(t_data *shell, t_command *cmd)
{
	int		argc;
	char	*path;

	if (!cmd || !shell || !cmd->command || !cmd->args || !shell->env)
		return (EXIT_FAILURE);
	argc = 0;
	while (cmd->args[argc])
		++argc;
	path = NULL;
	if (check_error(cmd, argc, shell, &path) || !set_oldpwd(shell))
		return (EXIT_FAILURE);
	if (argc == 1 || (argc == 2 && ft_strcmp(cmd->args[1], "--")))
		return (switch_dir(path, shell, 0));
	if (argc == 2 && ft_strcmp(cmd->args[1], "-"))
		return (switch_dir(path, shell, 1));
	if (argc == 3 && ft_strcmp(cmd->args[1], "--")
		&& ft_strcmp(cmd->args[2], "-"))
		return (switch_dir(path, shell, 1));
	return (switch_dir(path, shell, 2));
}
