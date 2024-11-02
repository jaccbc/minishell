/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 16:45:24 by joandre-          #+#    #+#             */
/*   Updated: 2024/11/01 21:04:35 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//returna o token type
static int	get_type(const char *s, t_token *last)
{
	if (!s || !*s)
		return (INVALID);
	if (ft_strncmp(s, "|", 1) == 0)
		return (PIPE);
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
	if (last->type != PIPE)
		return (ARG);
	return (COMMAND);
}

//cria um token (split + token type)
//returna o numero de bytes do split
static int	add_split(const char *s, size_t len, t_token **lst)
{
	t_token	*new;

	new = ft_calloc(1, sizeof(t_token));
	if (!new)
		return (0);
	new->str = ft_substr(s, 0, len);
	if (!new->str)
		return (free(new), 0);
	new->type = get_type(new->str, lstiter_token(*lst, DOWN, 0));
	new->next = NULL;
	new->prev = NULL;
	lstadd_token(lst, new);
	return (ft_strlen(new->str));
}

static int	split_quote(const char *s, char *quote, t_token **lst)
{
	char	*split;

	if (!s || !quote)
		return (0);
	split = quote;
	while (*split)
		if (*(++split) == *quote)
			break ;
	while (*split)
	{
		if (is_type(UNQUOTE, ++split))
			break ;
		else if (is_type(QUOTE, split))
		{
			quote = split;
			while (*split)
				if (*(++split) == *quote)
					break ;
		}
	}
	return (add_split(s, split - s, lst));
}

//calcula os bytes a copiar com pointer arithmetic
//returna o numero de bytes copiados
static int	split(int type, const char *s, t_token **lst)
{
	char	*str;

	str = (char *)s;
	if (type == QUOTE)
		return (split_quote(s, str, lst));
	if (type == UNQUOTE)
	{
		while (*str && !is_type(UNQUOTE, str))
		{
			if (is_type(QUOTE, str))
				return (split_quote(s, str, lst));
			if (*(++str) == '\n')
				break ;
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
			return (lstdel_token(lst), NULL);
	}
	return (lst);
}
