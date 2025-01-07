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

// receives the new buffer *str, original string *s, and the quote char q
// writes the new string without quotes ' "
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

// deletes closed quotes in the strings within the nodes of the list
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
