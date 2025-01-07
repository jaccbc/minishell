/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 17:42:30 by joandre-          #+#    #+#             */
/*   Updated: 2025/01/07 16:22:31 by vamachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Function to print error messages with optional quotes around the detail
static void	err_msg(char *msg, char *detail)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(msg, STDERR_FILENO);
	ft_putstr_fd(" `", STDERR_FILENO);
	ft_putstr_fd(detail, STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
	g_last_exit_code = 2;
}

// verifies if there are consecutive separators
static bool	consecutive_sep(t_token *lst)
{
	while (lst)
	{
		if ((lst->prev && ((lst->prev->type > PIPE && lst->type >= PIPE)))
			|| (!lst->next && lst->type == PIPE))
		{
			err_msg("syntax error near unexpected token ", lst->str);
			return (true);
		}
		else if (lst->type > PIPE && !lst->next)
		{
			err_msg("syntax error near unexpected token ", "newline");
			return (true);
		}
		lst = lst->next;
	}
	return (false);
}

// verifies if there are unclosed quotes within the string
static bool	quotes_verify(t_token *current)
{
	bool	s_quotes;
	bool	d_quotes;
	char	*str;

	s_quotes = false;
	d_quotes = false;
	while (current)
	{
		str = current->str;
		while (*str)
		{
			if (*str == '\"' && !s_quotes)
				d_quotes = !d_quotes;
			else if (*str == '\'' && !d_quotes)
				s_quotes = !s_quotes;
			str++;
		}
		current->s_quotes = s_quotes;
		current->d_quotes = d_quotes;
		current = current->next;
	}
	return (s_quotes || d_quotes);
}

// casts the error corresponding to each of the unclosed quotes
static void	unclosed_quotes(t_token *current)
{
	while (current)
	{
		if (current->s_quotes)
		{
			err_msg("unexpected EOF while looking for matching ", "\'");
			break ;
		}
		else if (current->d_quotes)
		{
			err_msg("unexpected EOF while looking for matching ", "\"");
			break ;
		}
		current = current->next;
	}
}

// analysis of some syntax errors
bool	syntax_error(t_token *lst)
{
	if (lst->type == PIPE)
	{
		err_msg("syntax error near unexpected token ", lst->str);
		return (false);
	}
	if (consecutive_sep(lst))
		return (false);
	if (quotes_verify(lst))
		return (unclosed_quotes(lst), false);
	return (true);
}
