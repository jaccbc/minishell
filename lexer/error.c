/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 17:42:30 by joandre-          #+#    #+#             */
/*   Updated: 2024/10/21 01:08:10 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	err_msg(char *msg, char *detail, bool in_quotes)
{
	char	*err_msg;

	err_msg = ft_strjoin("minishell: ", msg);
	if (in_quotes)
		err_msg = ft_strjoin(err_msg, "`");
	err_msg = ft_strjoin(err_msg, detail);
	if (in_quotes)
		err_msg = ft_strjoin(err_msg, "'");
	ft_putendl_fd(err_msg, STDERR_FILENO);
	free(err_msg);
}

// verifica se existem separadores seguidos
bool	consecutive_sep(t_token *lst)
{
	t_token	*current;

	current = lst;
	while (current)
	{
		if (current->prev && ((current->prev->type > PIPE
					&& current->type >= PIPE) || (current->prev->type == VAR
					&& current->type == PIPE)))
			return (err_msg("syntax error near unexpected token ", current->str,
					true), true);
		else if (current->type > PIPE && !current->next)
			return (err_msg("syntax error near unexpected token ", "newline",
					true), true);
		current = current->next;
	}
	return (false);
}

// analisa alguns erros de sintaxe e recorre
bool	syntax_error(t_token *lst)
{
	t_token	*current;

	current = lst;
	if (current->type == PIPE)
		return (err_msg("syntax error near unexpected token ", current->str,
				true), false);
	if (consecutive_sep(current))
		return (false);
	return (true);
}
