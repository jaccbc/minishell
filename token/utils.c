/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 22:10:33 by joandre-          #+#    #+#             */
/*   Updated: 2024/10/09 02:36:53 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	is_type(int type, int c)
{
	if (type == UNQUOTE)
		return (c == ' ' || c == '|' || c == '<' || c == '>');
	if (type == DELIMIT)
		return (c == '>' || c == '<' || c == '|');
	return (false);
}


//retorna o token numero i da lst
//DOWN 0 == last_token | UP 0 == first_token
t_token	*lstiter_token(t_token *lst, int type, size_t i)
{
	if (!lst)
		return (NULL);
	if (i == 0)
		i = ULLONG_MAX;
	if (type == DOWN)
	{
		while (lst->next && i--)
			lst = lst->next;
	}
	else if (type == DOWN)
	{
		if (lst->prev && i--)
			lst = lst->prev;
	}
	return (lst);
}

void	free_token(t_token *lst)
{
	t_token	*temp;

	while (lst != NULL)
	{
		temp = lst;
		lst = lst->next;
		free(temp->str);
		free(temp);
	}
}
