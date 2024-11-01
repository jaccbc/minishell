/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_files.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vamachad <vamachad@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 20:00:19 by vamachad          #+#    #+#             */
/*   Updated: 2024/11/01 01:21:59 by joandre-         ###   ########.fr       */
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

// Verify all redirection files for access errors
bool	check_files(t_token *lst)
{
	while (lst)
	{
		if (lst->type >= RED_IN)
		{
			if (lst->next == NULL)
				return (false);
			if (lst->type == RED_IN && access(lst->next->str, R_OK))
			{
				minishell_errmsg(lst->next->str, strerror(errno));
				return (false);
			}
			else if (lst->type >= RED_OUT && access(lst->next->str, F_OK) == 0)
			{
				if (access(lst->next->str, W_OK))
				{
					minishell_errmsg(lst->next->str, strerror(errno));
					return (false);
				}
			}
		}
		lst = lst->next;
	}
	return (true);
}
