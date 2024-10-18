/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 01:34:20 by joandre-          #+#    #+#             */
/*   Updated: 2024/10/18 02:52:53 by vamachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Função recebe uma string e cria um novo buffer
// Copia a string toda menos os 2 quote characters
// Retorna a nova string e dá free na recebida
static char	*get_unquote(char *s)
{
	char	*str;
	char	*q;
	int		i;
	int		k;

	if (!s)
		return (s);
	str = ft_calloc(ft_strlen(s), sizeof(char));
	if (!str)
		return (NULL);
	q = s;
	while (*q && !(*q == '\'' || *q == '\"'))
		++q;
	i = 0;
	k = -1;
	while (s[++k])
	{
		if (s[k] == *q)
			continue ;
		str[i++] = s[k];
	}
	return (free(s), str);
}

// Função inicia syntax analysis
bool	check_syntax(t_data *shell)
{
	int		i;
	t_token	*current;

	current = shell->lst;
	if (!syntax_error(shell->lst))
		return (free_token(shell->lst), false);
	var_expander(shell);
	while (current)
	{
		i = -1;
		while (current->str[++i])
		{
			if ((current->str[i] == '\'' || current->str[i] == '\"')
				&& is_type(QUOTE, &current->str[i]))
			{
				current->str = get_unquote(current->str);
				break ;
			}
		}
		current = current->next;
	}
	current = shell->lst;
	return (true);
}
