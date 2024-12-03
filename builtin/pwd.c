#include "../minishell.h"

int	ft_pwd(t_data *shell)
{
	char	buf[4096];
	char	*err;
	char	*cwd;

	if (!shell->env)
		return (EXIT_FAILURE);
	while (*shell->env)
	{
		if (ft_strncmp(*shell->env, "PWD", 3) == 0)
		{
			ft_putendl_fd((*shell->env) + 4, STDOUT_FILENO);
			return (EXIT_SUCCESS);
		}
		shell->env++;
	}
	cwd = getcwd(buf, 4096);
	if (cwd)
	{
		ft_putendl_fd(cwd, STDOUT_FILENO);
		return (EXIT_SUCCESS);
	}
	err = mini_errmsg("pwd", NULL, strerror(errno), true);
	ft_putendl_fd(err, STDERR_FILENO);
	free(err);
	return (EXIT_FAILURE);
}