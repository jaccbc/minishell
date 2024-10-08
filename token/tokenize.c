/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 16:45:24 by joandre-          #+#    #+#             */
/*   Updated: 2024/10/08 02:16:52 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

//get_type() retorna o token_type
static int	get_type(char *s, t_token *last)
{
	if (!s || !*s)
		return (INVALID);
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
	if (!last)
		return (COMMAND);
	if (ft_strncmp(s, "-", 1) == 0)
		return (FLAG);
	while (ft_isprint(*s++))
		if (*s == '\0' && last->type != PIPE)
			return (ARG);
	return (COMMAND);
}

static t_token	*create_token(t_list *split, t_token *last)
{
	t_token	*node;

	node = malloc(sizeof(t_token));
	if (!node)
		return (NULL);
	node->str = split->content;
	node->type = get_type(node->str, last);
	node->next = NULL;
	node->prev = NULL;
	return (node);
}

//convergência
t_token	*tokenize(char *s)
{
	t_token	*lst;
	t_token	*new;
	t_list	*split;
	t_list	*clean;

	if (!s)
		return (NULL);
	split = splitter(s);
	if (!split)
		return (NULL);
	lst = NULL;
	while (split)
	{
		new = create_token(split, last_token(lst));
		if (!new)
			return (ft_lstdelone(split, free), free_token(lst), NULL);
		lstadd_token(&lst, new);
		clean = split;
		split = split->next;
		free(clean);
	}
	return (lst);
}
