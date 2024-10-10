/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 16:45:24 by joandre-          #+#    #+#             */
/*   Updated: 2024/10/10 01:59:06 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//adiciona um token na lista
static void	lstadd_token(t_token **lst, t_token *new)
{
	t_token	*current;

	if (!lst || !new)
		return ;
	current = *lst;
	if (current)
	{
		while (current->next)
			current = current->next;
		current->next = new;
		new->prev = current;
	}
	else
		*lst = new;
}

//returna o token type
static int	get_type(char *s, t_token *last)
{
	if (!s || !*s)
		return (INVALID);
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
	if (ft_strncmp(s, "|", 1) == 0)
		return (PIPE);
	if (ft_strncmp(s, "-", 1) == 0)
		return (FLAG);
	if (last)
		while (*s)
			if (*s++ == '\0' && last->type != PIPE)
				return (ARG);
	return (COMMAND);
}

//cria um token (split + token type)
//returna o numero de bytes do split
static int	add_split(char *s, size_t len, t_token **lst)
{
	t_token	*new;
	char	*split;

	split = ft_substr(s, 0, len);
	if (!split)
		return (0);
	new = malloc(sizeof(t_token));
	if (!new)
		return (0);
	new->str = split;
	new->type = get_type(split, lstiter_token(*lst, DOWN, 0));
	new->next = NULL;
	new->prev = NULL;
	lstadd_token(lst, new);
	return (ft_strlen(new->str));
}

//calcula os bytes a copiar com pointer arithmetic
//returna o numero de bytes copiados
static int	split(int type, char *s, t_token **lst)
{
	char	*str;

	str = s;
	if (type == QUOTE)
	{
		++str;
		while (*str && *str != *s)
			++str;
		return (add_split(s, ++str - s, lst));
	}
	else if (type == UNQUOTE)
	{
		while (*str && !is_type(UNQUOTE, *str))
			++str;
		return (add_split(s, str - s, lst));
	}
	else if (type == DELIMIT)
	{
		if (ft_strncmp(s, ">>", 2) == 0 || ft_strncmp(s, "<<", 2) == 0)
			return (add_split(s, 2, lst));
		else if (is_type(DELIMIT, *s))
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
