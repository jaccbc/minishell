/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 16:45:24 by joandre-          #+#    #+#             */
/*   Updated: 2024/10/05 21:31:26 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

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

//cria um token novo
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

t_token	*last_token(t_token *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

//convergência
t_token	*tokenizer(char *s)
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

// DEBUG PART WITH MAIN
//auxiliar para imprimir a lista
//só para sair o tipo e não o int que representa o tipo
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

//limpa a lista
void	free_tokens(t_token **lst)
{
	t_token	*current;
	t_token	*temp;

	current = *lst;
	while (current != NULL)
	{
		temp = current;
		current = current->next;
		free(temp->str);
		free(temp);
	}
	*lst = NULL;
}

int	main(void)
{
	t_data	shell;
	char	*prompt = "echo $HOME >> /path/to/file";

	shell.lst = tokenizer(prompt);
	print_list(shell.lst);
	free_tokens(&shell.lst);
	return (0);
}
