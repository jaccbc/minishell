/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_files.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vamachad <vamachad@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 20:00:19 by vamachad          #+#    #+#             */
/*   Updated: 2024/11/04 22:08:58 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Function to output error messages in a Bash-like format
void	minishell_errmsg(char *filename, char *error_message)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(filename, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putendl_fd(error_message, STDERR_FILENO);
}

// Unified function to open a file
// handle error reporting with strerror(errno)
static bool	open_file(t_token *token, int flags, int mode)
{
	int	fd;

	fd = open(token->next->str, flags, mode);
	if (fd == -1)
	{
		minishell_errmsg(token->next->str, strerror(errno));
		return (false);
	}
	close(fd);
	fd = -1;
	return (true);
}

// Verify all redirection files for access errors
bool	check_files(t_token *token)
{
	while (token)
	{
		if (token->type == RED_IN && access(token->next->str, R_OK) != 0)
			return (minishell_errmsg(token->next->str, strerror(errno)), false);
		else if (token->type == APPEND)
		{
			if (!open_file(token, O_WRONLY | O_CREAT | O_APPEND, 0644))
				return (false);
		}
		else if (token->type >= RED_OUT)
		{
			if (!open_file(token, O_WRONLY | O_CREAT | O_TRUNC, 0644))
				return (false);
		}
		token = token->next;
	}
	return (true);
}
