/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 01:20:35 by joandre-          #+#    #+#             */
/*   Updated: 2024/11/30 19:18:16 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// retorna o token numero i da lst
// DOWN 0 == last_token | UP 0 == first_token
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
	else if (type == UP)
	{
		while (lst->prev && i--)
			lst = lst->prev;
	}
	return (lst);
}

// adiciona um token na lista
void	lstadd_token(t_token **lst, t_token *new)
{
	t_token	*current;

	if (!lst || !new)
		return ;
	current = *lst;
	if (current)
	{
		while (current->next)
			current = current->next;
		current->next = new;
		new->prev = current;
	}
	else
		*lst = new;
}

void	lstdel_token(t_token *lst)
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

bool	is_type(int type, const char *s)
{
	const char	*str;

	if (!s || !*s)
		return (false);
	if (type == UNQUOTE)
		return (*s == ' ' || *s == '|' || *s == '<' || *s == '>');
	if (type == DELIMIT)
		return (*s == '|' || *s == '<' || *s == '>');
	if (type == QUOTE && (*s == '\'' || *s == '\"'))
	{
		str = s + 1;
		while (*str)
			if (*str++ == *s)
				return (true);
	}
	if (type == VAR)
		return (*s == '$' && (ft_isalnum(*(s + 1)) || *(s + 1) == '_' || *(s + 1) == '?'));
	if (type == PATH)
		return (ft_strncmp(s, "/", 1) == 0 || ft_strncmp(s, "../", 3) == 0
				|| ft_strncmp(s, "~/", 2) == 0);
	return (false);
}

bool	ft_strcmp(char *line, char *delimiter)
{
	if (!line || !delimiter)
		return (false);
	if (ft_strlen(line) != ft_strlen(delimiter))
		return (false);
	while (*line && *delimiter)
		if (*line++ != *delimiter++)
			return (false);
	return (true);
}
