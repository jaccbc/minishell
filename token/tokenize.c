/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 16:45:24 by joandre-          #+#    #+#             */
/*   Updated: 2024/10/12 00:45:28 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//returna o token type
static int	get_type(char *s, t_token *last)
{
	if (!s || !*s)
		return (INVALID);
	if (ft_strncmp(s, "|", 1) == 0)
		return (PIPE);
	if (ft_strncmp(s, "$", 1) == 0)
		return (VAR);
	if (ft_strncmp(s, ">>", 2) == 0)
		return (APPEND);
	if (ft_strncmp(s, "<<", 2) == 0)
		return (HEREDOC);
	if (ft_strncmp(s, "<", 1) == 0)
		return (RED_IN);
	if (ft_strncmp(s, ">", 1) == 0)
		return (RED_OUT);
	if (!last)
		return (COMMAND);
	if (ft_strncmp(s, "-", 1) == 0)
		return (FLAG);
	while (*s)
	{
		if (last->type != PIPE)
			return (ARG);
		++s;
	}
	return (COMMAND);
}

//cria um token (split + token type)
//returna o numero de bytes do split
static int	add_split(char *s, size_t len, t_token **lst)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (0);
	ft_bzero(new, sizeof(t_token));
	new->str = ft_substr(s, 0, len);
	if (!new->str)
		return (free(new), 0);
	new->type = get_type(new->str, lstiter_token(*lst, DOWN, 0));
	lstadd_token(lst, new);
	return (ft_strlen(new->str));
}

int	quote_len(char *s, char *str, t_token **lst)
{
	if (!s || !str)
		return (0);
	++str;
	while (*str && *str != *s)
		++str;
	while (*str && *str != ' ')
		++str;
	return (add_split(s, ++str - s, lst));
}

//calcula os bytes a copiar com pointer arithmetic
//returna o numero de bytes copiados
static int	split(int type, char *s, t_token **lst)
{
	char	*str;

	str = s;
	if (type == QUOTE)
		return (quote_len(s, str, lst));
	else if (type == UNQUOTE)
	{
		while (*str && !is_type(UNQUOTE, str))
		{
			++str;
			if (*str == '\'' || *str == '\"')
				if (is_type(QUOTE, str))
					return (quote_len(s, ++str, lst));
		}
		return (add_split(s, str - s, lst));
	}
	else if (type == DELIMIT)
	{
		if (ft_strncmp(s, ">>", 2) == 0 || ft_strncmp(s, "<<", 2) == 0)
			return (add_split(s, 2, lst));
		else if (is_type(DELIMIT, s))
			return (add_split(s, 1, lst));
	}
	return (0);
}

//cria uma lista de tokens
t_token	*tokenize(char *s)
{
	t_token	*lst;
	char	*i;

	if (!s)
		return (NULL);
	lst = NULL;
	while (*s)
	{
		i = s;
		if (*s == ' ')
			++s;
		else if (*s == '\'' || *s == '\"')
			s += split(QUOTE, s, &lst);
		else if (*s == '|' || *s == '>' || *s == '<')
			s += split(DELIMIT, s, &lst);
		else
			s += split(UNQUOTE, s, &lst);
		if (i == s)
			return (free_token(lst), NULL);
	}
	return (lst);
}
