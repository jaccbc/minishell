/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   special_var.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 00:52:14 by joandre-          #+#    #+#             */
/*   Updated: 2024/10/22 03:05:11 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//função responsável por apagar os dollars não expansiveis (fora de quotes)
void	del_dollar(t_token *lst)
{
	char	*q;
	int		i;

	while (lst)
	{
		i = 0;
		while (lst->str[i] && ft_strchr(&lst->str[i], '$'))
		{
			q = &lst->str[i];
			if (is_type(QUOTE, &lst->str[i]))
				while (lst->str[++i])
					if (lst->str[i] == *q)
						break ;
			if (lst->str[i++] == '$')
			{
				if (lst->str[i] != '\0')
				{
					q = &lst->str[i - 1];
					while (*q)
						*q++ = lst->str[i++];
				}
			}
		}
		lst = lst->next;
	}
}
