/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 22:10:33 by joandre-          #+#    #+#             */
/*   Updated: 2024/10/05 22:45:28 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//imprime a lista de tokens
void	print_list(t_token *lst)
{
	t_token	*current;
	int		i;

	current = lst;
	i = 0;
	while (current != NULL)
	{
		printf("TOKEN Nº%d\n[%p]\nstr=[%s]\ntype[%s]\nprev=[%p]\nnext=[%p]\n\n",
			++i, current, current->str, token_name(current->type),
			current->prev, current->next);
		current = current->next;
	}
}

//retorna o nome do token_type
char	*token_name(int type)
{
	if (type == 1)
		return ("COMMAND");
	if (type == 2)
		return ("FLAG");
	if (type == 3)
		return ("ARG");
	if (type == 4)
		return ("VAR");
	if (type == 5)
		return ("RED_IN");
	if (type == 6)
		return ("RED_OUT");
	if (type == 7)
		return ("PIPE");
	if (type == 8)
		return ("APPEND");
	if (type == 9)
		return ("HEREDOC");
	return ("INVALID");
}
