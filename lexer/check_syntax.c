/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 01:34:20 by joandre-          #+#    #+#             */
/*   Updated: 2024/11/08 02:45:22 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	check_heredoc_expansion(t_token *lst)
{
	int	i;

	while (lst)
	{
		if (lst->type == HEREDOC)
		{
			lst = lst->next;
			i = -1;
			while (lst->str[++i])
			{
				if (lst->str[i] == '\'' || lst->str[i] == '\"')
				{
					lst->s_quotes = true;
					lst->d_quotes = true;
					return ;
				}
			}
		}
		lst = lst->next;
	}
}

// Expande as variáveis em shell->lst
// varifica is_type(VAR), expande e recomeça o check com a string atualizada
static void	var_expander(t_data *shell)
{
	int		i;
	t_token	*t;

	t = shell->lst;
	while (t)
	{
		i = 0;
		while (t->str[i] && ft_strchr(&t->str[i], '$'))
		{
			if (t->str[i] == '\'' && is_type(QUOTE, &t->str[i]))
				while (t->str[++i])
					if (t->str[i] == '\'')
						break ;
			if (t->str[i] == '\"' && is_type(QUOTE, &t->str[i]))
				while (t->str[i++])
					if (t->str[i] == '\"' || is_type(VAR, &t->str[i]))
						break ;
			if (is_type(VAR, &t->str[i++]))
			{
				expander(&t->str, shell->env, &t->str[i]);
				continue ;
			}
		}
		t = t->next;
	}
}

//função inicia syntax analysis
bool	check_syntax(t_data *shell)
{
	if (!shell->lst)
		return (false);
	if (!syntax_error(shell->lst))
		return (false);
	print_list(shell->lst);
	check_heredoc_expansion(shell->lst);
	var_expander(shell);
	del_dollar(shell->lst);
	if (!del_quote(shell->lst))
		return (false);
	return (true);
}
