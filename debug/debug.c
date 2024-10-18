/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 22:10:33 by joandre-          #+#    #+#             */
/*   Updated: 2024/10/18 02:53:52 by vamachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// imprime a lista de tokens
void	print_list(t_token *lst)
{
	t_token	*current;
	int		i;

	current = lst;
	i = 0;
	printf("*** DEBUG PRINT_LIST ***\n");
	while (current != NULL)
	{
		printf("TOKEN Nº%d\n[%p]\n", ++i, current);
		printf("str=[%s]\ntype=[%s]\nprev=[%p]\nnext=[%p]\n\n", current->str,
			token_name(current->type), current->prev, current->next);
		current = current->next;
	}
	printf("*** DEBUG PRINT_LIST END***\n");
}

void	print_array(char **arr)
{
	int	i;

	i = -1;
	while (arr[++i])
	{
		printf("%s\n", arr[i]);
	}
}

// retorna o nome do token_type
char	*token_name(int type)
{
	if (type == COMMAND)
		return ("COMMAND");
	if (type == FLAG)
		return ("FLAG");
	if (type == ARG)
		return ("ARG");
	if (type == VAR)
		return ("VAR");
	if (type == RED_IN)
		return ("RED_IN");
	if (type == RED_OUT)
		return ("RED_OUT");
	if (type == PIPE)
		return ("PIPE");
	if (type == APPEND)
		return ("APPEND");
	if (type == HEREDOC)
		return ("HEREDOC");
	return ("INVALID");
}
