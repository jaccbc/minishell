#include "includes/minishell.h"
#include <readline/history.h>

//será parte do modo interactivo, o loop que pede o input do utilizador
//que será mais tarde lexado, parsado e executado
int	main(void)
{
	char	*user_input;

	while (1)
	{
		user_input = readline(PROMPT);
		add_history(user_input);
		printf("%s\n", user_input);

		free(user_input);
	}
}