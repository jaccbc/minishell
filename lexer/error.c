/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 17:42:30 by joandre-          #+#    #+#             */
/*   Updated: 2024/10/18 02:51:28 by vamachad         ###   ########.fr       */
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

// verifica se existem quotes que foram deixados abertos, e recategoriza
// determinado token que esteja em double quotes e que contenha uma variável
// real lá dentro.
bool	quotes_verify(t_token *current)
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
bool	unclosed_quotes(t_token *current)
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
	t_token	*current;

	current = lst;
	if (current->type == PIPE)
		return (err_msg("syntax error near unexpected token ", current->str,
				true), false);
	if (consecutive_sep(current))
		return (false);
	if (quotes_verify(current))
		return (unclosed_quotes(current));
	return (true);
}
