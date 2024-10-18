/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vamachad <vamachad@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 03:02:37 by vamachad          #+#    #+#             */
/*   Updated: 2024/10/18 03:02:39 by vamachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Procura em env a variável e retorna o valor
char	*get_value(t_data *shell, char *s)
{
	int		i;
	char	*var;
	char	*value;
	int		len;

	var = extract_var_name(s);
	if (!var)
		return (NULL);
	i = -1;
	while (shell->env[++i])
		if (ft_strncmp(var, shell->env[i], ft_strlen(var)) == 0)
			break ;
	if (shell->env[i] == NULL)
		return (NULL);
	len = ft_strlen(var);
	free(var);
	value = ft_strdup(shell->env[i] + len);
	return (value);
}

// Decide se se elimina a variável, no caso desta não existir em env,
// ou se se substitui pelo seu valor
void	sub_var(t_token *lst, int index, char *value)
{
	if (value == NULL)
	{
		erase_var(lst, index);
	}
	else
	{
		erase_and_replace_var(lst, index, value);
		free_ptr(value);
	}
}

// Expande as variáveis em shell->lst
void	var_expander(t_data *shell)
{
	t_token	*current;
	int		i;
	bool	s_quotes;
	char	*value;

	current = shell->lst;
	while (current)
	{
		s_quotes = false;
		if (there_is_dollar(current->str))
		{
			i = -1;
			while (current->str[++i])
			{
				s_quotes = is_in_s_quotes(&current->str[i], s_quotes);
				if (current->str[i] == '$' && !nxt_ch_sep(current->str[i + 1])
					&& !s_quotes && !var_in_quotes(&current->str[i]))
				{
					value = get_value(shell, &current->str[i]);
					sub_var(current, i, value);
				}
			}
		}
		current = current->next;
	}
}
