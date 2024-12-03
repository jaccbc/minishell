#include "../minishell.h"

int	ft_env(t_data *shell)
{
	int	i;
	char	*err;

	if(shell->command->args && shell->command->args[1])
	{
		err = mini_errmsg("env", NULL, "too many arguments", true);
		ft_putendl_fd(err, STDERR_FILENO);
		free(err);
		return (EXIT_FAILURE);
	}
	if (!shell || !shell->env)
		return (EXIT_FAILURE);
	i = -1;
	while (shell->env[++i])
		ft_putendl_fd(shell->env[i], STDOUT_FILENO);
	return (EXIT_SUCCESS);
}