#include "../minishell.h"

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
