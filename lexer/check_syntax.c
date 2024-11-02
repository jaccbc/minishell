/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 01:34:20 by joandre-          #+#    #+#             */
/*   Updated: 2024/11/02 03:27:46 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
	var_expander(shell);
	del_dollar(shell->lst);
	if (!del_quote(shell->lst))
		return (false);
	if (!check_files(shell->lst))
		return (false);
	return (true);
}
