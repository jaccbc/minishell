/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 22:10:33 by joandre-          #+#    #+#             */
/*   Updated: 2024/10/08 04:01:23 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	is_delimit(int type, int c)
{
	if (type == UNQUOTE)
		return (c == ' ' || c == '|' || c == '<' || c == '>');
	return (false);
}

t_token	*last_token(t_token *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
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
