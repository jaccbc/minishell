/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_redirections.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vamachad <vamachad@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 20:00:19 by vamachad          #+#    #+#             */
/*   Updated: 2024/10/28 20:00:24 by vamachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Function to output error messages in a Bash-like format
void	err_msg_basic(char *filename, char *error_message)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(filename, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putendl_fd(error_message, STDERR_FILENO);
}

// Unified function to open a file and handle error reporting with strerror(errno)
static bool	open_file_with_error(t_token *token, int flags, int mode)
{
	int	fd;

	fd = open(token->next->str, flags, mode);
	if (fd == -1)
	{
		err_msg_basic(token->next->str, strerror(errno));
		return (false);
	}
	close(fd);
	return (true);
}

// Validate redirection type with error messaging
static bool	validate_redirection_type(t_token *token)
{
	if (token->type == RED_IN)
		return (open_file_with_error(token, O_RDONLY, 0));
	else if (token->type == RED_OUT)
		return (open_file_with_error(token, O_WRONLY | O_CREAT | O_TRUNC,
				0644));
	else if (token->type == APPEND)
		return (open_file_with_error(token, O_WRONLY | O_CREAT | O_APPEND,
				0644));
	return (true);
}

// Verify all redirection files for access errors
bool	validate_redirections(t_data *shell)
{
	t_token	*token;

	token = shell->lst;
	while (token)
	{
		if (token->type == RED_IN || token->type == RED_OUT
			|| token->type == APPEND)
		{
			if (!validate_redirection_type(token))
				return (false);
		}
		token = token->next;
	}
	return (true);
}
