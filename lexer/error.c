/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 17:42:30 by joandre-          #+#    #+#             */
/*   Updated: 2024/11/01 01:57:51 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Function to print error messages with optional quotes around the detail
static void	err_msg(char *msg, char *detail, bool in_quotes)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(msg, STDERR_FILENO);
	if (in_quotes)
		ft_putstr_fd(" `", STDERR_FILENO);
	else
		ft_putstr_fd(" ", STDERR_FILENO);
	ft_putstr_fd(detail, STDERR_FILENO);
	if (in_quotes)
		ft_putstr_fd("'", STDERR_FILENO);
	ft_putchar_fd('\n', STDERR_FILENO);
}

// verifica se existem separadores seguidos
static bool	consecutive_sep(t_token *lst)
{
	while (lst)
	{
		if ((lst->prev && ((lst->prev->type > PIPE && lst->type >= PIPE)))
			|| (!lst->next && lst->type == PIPE))
		{
			err_msg("syntax error near unexpected token ", lst->str, true);
			return (true);
		}
		else if (lst->type > PIPE && !lst->next)
		{
			err_msg("syntax error near unexpected token ", "newline", true);
			return (true);
		}
		lst = lst->next;
	}
	return (false);
}

// verifica se existem quotes que foram deixados abertos, e recategoriza
// determinado token que esteja em double quotes e que contenha uma variável
// real lá dentro.
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

// lança o erro correspondente para unclosed quotes
static bool	unclosed_quotes(t_token *current)
{
	if (current->s_quotes)
		err_msg("unexpected EOF while looking for matching ", "\'", true);
	else if (current->d_quotes)
		err_msg("unexpected EOF while looking for matching ", "\"", true);
	err_msg("syntax error: unexpected end of file", "", false);
	return (false);
}

// analisa alguns erros de sintaxe e recorre
bool	syntax_error(t_token *lst)
{
	if (lst->type == PIPE)
	{
		err_msg("syntax error near unexpected token ", lst->str, true);
		return (false);
	}
	if (consecutive_sep(lst))
		return (false);
	if (quotes_verify(lst))
		return (unclosed_quotes(lst));
	return (true);
}
