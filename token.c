/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 16:45:24 by joandre-          #+#    #+#             */
/*   Updated: 2024/10/05 23:24:43 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//adiciona o node ao fim da lista de tokens
static void	lstadd_token(t_token **lst, t_token *new)
{
	t_token	*current;

	if (!lst || !new)
		return ;
	current = *lst;
	if (current)
	{
		while (current->next != NULL)
			current = current->next;
		current->next = new;
		new->prev = current;
	}
	else
		*lst = new;
}

// get_type() retorna o token_type
// baseando-se na string e no último token da lista
static int	arg_or_cmd(char *s, t_token *last)
{
	int	i;

	if (!last)
		return (COMMAND);
	i = 0;
	while (ft_isprint(s[i++]))
		if (s[i] == '\0' && last->type != PIPE)
			return (ARG);
	return (COMMAND);
}

static int	get_type(char *s, t_token *last)
{
	if (!s || !*s)
		return (INVALID);
	if (ft_strncmp(s, "-", 1) == 0)
		return (FLAG);
	if (ft_strncmp(s, "$", 1) == 0)
		return (VAR);
	if (ft_strncmp(s, ">>", 2) == 0)
		return (APPEND);
	if (ft_strncmp(s, "<<", 2) == 0)
		return (HEREDOC);
	if (ft_strncmp(s, "<", 1) == 0)
		return (RED_IN);
	if (ft_strncmp(s, ">", 1) == 0)
		return (RED_OUT);
	if (ft_strncmp(s, "|", 1) == 0)
		return (PIPE);
	return (arg_or_cmd(s, last));
}

static t_token	*create_token(char *s, t_token *last)
{
	t_token	*node;

	node = malloc(sizeof(t_token));
	if (!node)
		return (NULL);
	node->str = s;
	node->type = get_type(s, last);
	node->next = NULL;
	node->prev = NULL;
	return (node);
}

//convergência
t_token	*tokenize(char *s)
{
	t_token	*lst;
	char	**arr;
	int		i;

	if (!s)
		return (NULL);
	lst = NULL;
	arr = ft_split(s, ' ');
	i = -1;
	while (arr[++i] != NULL)
		lstadd_token(&lst, create_token(arr[i], last_token(lst)));
	free(arr);
	return (lst);
}
