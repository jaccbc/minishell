/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 17:42:30 by joandre-          #+#    #+#             */
/*   Updated: 2024/10/14 19:39:00 by joandre-         ###   ########.fr       */
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
	free (err_msg);
}

bool	syntax_error(t_token *lst)
{
	if (!lst)
		return (true);
	if (lst->type == PIPE)
		return (err_msg("syntax error near unexpected token ", lst->str, true), true);
	if (lst->type == HEREDOC || lst->type == RED_IN || lst->type == RED_OUT || lst->type == APPEND)
		return (err_msg("syntax error near unexpected token ", "newline", true), true);
	while (lst)
	{
		if (lst->prev && lst->prev->type > PIPE && lst->type >= PIPE)
			return (err_msg("syntax error near unexpected token ", lst->str, true), true);
		else if (lst->type > PIPE && !lst->next)
			return (err_msg("syntax error near unexpected token ", "newline", true), true);
		lst = lst->next;
	}
	return (false);
}
/*
bool quotes_n_var(t_token *lst)
{
    t_token *current;
    bool s_quotes = false;
    bool d_quotes = false;

    current = lst;
    while (current)
    {
        int i = 0;
        while (current->str[i])
        {
            if (current->str[i] == '\"' && !s_quotes)
                d_quotes = !d_quotes;
            else if (current->str[i] == '\'' && !d_quotes)
                s_quotes = !s_quotes;
            if (current->str[i] == '$' && d_quotes)
                current->var_in_quotes = true;
            i++;
        }
        current = current->next;
    }
    return (s_quotes || d_quotes);
}*/
