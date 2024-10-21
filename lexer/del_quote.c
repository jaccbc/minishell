/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   del_quote.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 01:34:20 by joandre-          #+#    #+#             */
/*   Updated: 2024/10/21 01:47:51 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//recebe o novo buffer *str, string original *s e o quote char
//escreve a nova string sem quotes ' "
static void	write_buffer(char *str, const char *s, int q)
{
	int	count;

	while (*s)
	{
		count = 2;
		while (*s && count)
		{
			if (*s == q)
			{
				--count;
				++s;
			}
			else
				*str++ = *s++;
		}
		while (*s)
		{
			if (is_type(QUOTE, (char *)s))
			{
				q = *s;
				break ;
			}
			*str++ = *s++;
		}
	}
}

//elimina aspas " ' fechedas das strings da lista
bool	del_quote(t_token *lst)
{
	int		i;
	char	*unquote;

	while (lst)
	{
		i = -1;
		while (lst->str[++i])
		{
			if (is_type(QUOTE, &lst->str[i]))
			{
				unquote = ft_calloc(ft_strlen(lst->str), sizeof(char));
				if (unquote == NULL)
					return (false);
				write_buffer(unquote, lst->str, lst->str[i]);
				free(lst->str);
				lst->str = unquote;
				break ;
			}
		}
		lst = lst->next;
	}
	return (true);
}
