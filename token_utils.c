/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 22:10:33 by joandre-          #+#    #+#             */
/*   Updated: 2024/10/06 02:22:15 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*last_token(t_token *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

void	free_tokens(t_token *lst)
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
