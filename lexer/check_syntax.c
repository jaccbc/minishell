/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 01:34:20 by joandre-          #+#    #+#             */
/*   Updated: 2024/10/14 19:41:07 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//função recebe uma string e cria um novo buffer
//copia a string toda menos os 2 quote characters
//retorna a nova string e dá free na recebida
static char	*get_unquote(char *s)
{
	char	*str;
	char	*q;
	int		i;
	int		k;

	if (!s)
		return (s);
	str = ft_calloc(ft_strlen(s), sizeof(char));
	if (!str)
		return (NULL);
	q = s;
	while (*q && !(*q == '\'' || *q == '\"'))
		++q;
	i = 0;
	k = -1;
	while (s[++k])
	{
		if (s[k] == *q)
			continue ;
		str[i++] = s[k];
	}
	return (free(s), str);
}

//função inicia syntax analysis
bool	check_syntax(t_token *lst)
{
	int		i;

	if (syntax_error(lst))
		return (free_token(lst), false);
	while (lst)
	{
		i = -1;
		while (lst->str[++i])
		{
			if ((lst->str[i] == '\'' || lst->str[i] == '\"')
				&& is_type(QUOTE, &lst->str[i]))
			{
				lst->str = get_unquote(lst->str);
				break ;
			}
		}
		lst = lst->next;
	}
	return (true);
}
