/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   splitter.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vamachad <vamachad@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 16:53:29 by vamachad          #+#    #+#             */
/*   Updated: 2024/10/07 02:48:06 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static bool	is_delimit(char c)
{
	return (c == ' ' || c == '|' || c == '>' || c == '<');
}

//adiciona o split ao fim da lista
static int	add_token(char *s, unsigned int len, t_list **token)
{
	char	*str;
	t_list	*new;

	str = ft_substr(s, 0, len);
	if (!str)
		return (0);
	new = ft_lstnew(str);
	if (new)
		ft_lstadd_back(token, new);
	return (ft_strlen(str));
}

//mete as strings num array e limpa a lista
//retorna um array of char pointers (tokens)
static char	**array(t_list *token)
{
	t_list	*clean;
	char	**arr;
	int		i;

	if (!token)
		return (NULL);
	arr = malloc((ft_lstsize(token) + 1) * sizeof(char *));
	if (!arr)
		return (ft_lstdelone(token, free), NULL);
	i = 0;
	while (token)
	{
		arr[i++] = token->content;
		clean = token;
		token = token->next;
		free(clean);
	}
	arr[i] = NULL;
	return (arr);
}

//faz o split conforme o tipo (QUOTE|UNQUOTE|DELIMIT)
//retorna o numero the chars copiados
static int	handle(int split, char *s, t_list **token)
{
	char	*str;

	str = s;
	if (split == QUOTE)
	{
		++str;
		while (*str && *str != *s)
			++str;
		return (add_token(s, ++str - s, token));
	}
	else if (split == UNQUOTE)
	{
		while (*str)
			if (is_delimit(*(++str)))
				break ;
		return (add_token(s, str - s, token));
	}
	else if (split == DELIMIT)
	{
		if ((*s == '>' && *(s + 1) == '>') || (*s == '<' && *(s + 1) == '<'))
			return (add_token(s, 2, token));
		else if ((*s == '>') || (*s == '<') || (*s == '|'))
			return (add_token(s, 1, token));
	}
	return (-1);
}

// retorna os splits num array of pointers
char	**splitter(char *s)
{
	t_list	*token;

	token = NULL;
	while (*s)
	{
		if (*s == ' ')
			++s;
		if (*s == '\'' || *s == '\"')
			s += handle(QUOTE, s, &token);
		else if (*s == '|' || *s == '>' || *s == '<')
			s += handle(DELIMIT, s, &token);
		else
			s += handle(UNQUOTE, s, &token);
	}
	return (array(token));
}
