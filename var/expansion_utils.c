/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vamachad <vamachad@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 03:18:06 by vamachad          #+#    #+#             */
/*   Updated: 2024/10/18 03:18:09 by vamachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Os caracteres a seguir ao $ constituem uma variável válida?
bool	is_var_compliant(char c)
{
	if (ft_isalnum(c) == 0 && c != '_')
		return (false);
	else
		return (true);
}

// Tem o $ dentro da string algures?
bool	there_is_dollar(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
		if (str[i] == '$')
			return (true);
	return (false);
}

// Tem disto a seguir?
bool	nxt_ch_sep(char c)
{
	return (c == '$' || c == ' ' || c == '=' || c == '\0');
}

// É o caso "$"?
bool	var_in_quotes(char *s)
{
	if (*(s - 1) == '\"' && *(s + 1) == '\"')
		return (true);
	return (false);
}

// Está dentro de single quotes ou fora
bool	is_in_s_quotes(char *str, bool s_quotes)
{
	if (*str == '\'' && !s_quotes)
		s_quotes = true;
	else if (*str == '\'' && s_quotes)
		s_quotes = false;
	return (s_quotes);
}
