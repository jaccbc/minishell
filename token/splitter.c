/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   splitter.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vamachad <vamachad@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 16:53:29 by vamachad          #+#    #+#             */
/*   Updated: 2024/10/08 01:31:52 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static bool	is_delimit(char c)
{
	return (c == ' ' || c == '|' || c == '>' || c == '<');
}

//adiciona o split (str) ao fim da lista
static int	add_split(char *s, size_t len, t_list **lst)
{
	char	*str;
	t_list	*new;

	str = ft_substr(s, 0, len);
	if (!str)
		return (0);
	new = ft_lstnew(str);
	if (new)
		ft_lstadd_back(lst, new);
	return (ft_strlen(str));
}

//cria uma string conforme o tipo de split(QUOTE|UNQUOTE|DELIMIT)
//usa pointer arithmetic para calcular o numero de chars a copiar
//retorna o numero the chars copiados
static int	split(int type, char *s, t_list **lst)
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
		while (*str)
			if (is_delimit(*(++str)))
				break ;
		return (add_split(s, str - s, lst));
	}
	else if (type == DELIMIT)
	{
		if ((*s == '>' && *(s + 1) == '>') || (*s == '<' && *(s + 1) == '<'))
			return (add_split(s, 2, lst));
		else if ((*s == '>') || (*s == '<') || (*s == '|'))
			return (add_split(s, 1, lst));
	}
	return (0);
}

//retorna as strings (splits) num array of pointers
t_list	*splitter(char *s)
{
	t_list	*lst;

	lst = NULL;
	while (*s)
	{
		if (*s == ' ')
			++s;
		else if (*s == '\'' || *s == '\"')
			s += split(QUOTE, s, &lst);
		else if (*s == '|' || *s == '>' || *s == '<')
			s += split(DELIMIT, s, &lst);
		else
			s += split(UNQUOTE, s, &lst);
	}
	return (lst);
}
