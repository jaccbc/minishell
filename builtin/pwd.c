#include "../minishell.h"

int	ft_pwd(t_data *shell)
{
	int	i;

	if (!shell || shell->env)
		return (EXIT_FAILURE);
	i = -1;
	while (shell->env[++i])
	{
		if (ft_strncmp(shell->env[i], "PWD", 3) == 0)
			ft_putendl_fd(shell->env[i] + 4, 1);
	}
	return (EXIT_SUCCESS);
}