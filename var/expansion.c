/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vamachad <vamachad@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 03:02:37 by vamachad          #+#    #+#             */
/*   Updated: 2025/01/09 12:09:54 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static size_t	get_varlen(const char *s)
{
	size_t	len;

	if (s[0] == '?')
		return (1);
	len = 0;
	while (ft_isalnum(s[len]) || s[len] == '_')
		++len;
	return (len);
}

// searches for the variable in env and returns its value
// "ft_strchr(env[i], '=') + 1" returns everything after = in **env
static char	*get_value(t_data *shell, const char *s)
{
	int		i;
	char	*var;

	if (s[0] == '?')
		return (ft_itoa(shell->status));
	var = ft_calloc(get_varlen(s) + 2, sizeof(char));
	if (!var)
		return (NULL);
	i = 0;
	while (ft_isalnum(s[i]) || s[i] == '_')
	{
		var[i] = s[i];
		++i;
	}
	var[i] = '=';
	i = 0;
	while (shell->env[i] && ft_strncmp(var, shell->env[i], ft_strlen(var)))
		++i;
	free(var);
	if (shell->env[i] == NULL)
		return (NULL);
	return (ft_strchr(shell->env[i], '=') + 1);
}

//measures the length of the strings and allocates memory for the expanded str
static void	*create_buffer(char *str, t_data *shell, char *var, char **expand)
{
	size_t	len;

	len = 1;
	*expand = get_value(shell, var);
	if (*expand)
		len += ft_strlen(*expand);
	len += ft_strlen(str) - get_varlen(var);
	return (ft_calloc(len, sizeof(char)));
}

//variable expansion
//1st while loop copies everything until it finds the $
//2nd while loop copies the expansion if it exists
//3rd while loop copies the rest of the string
void	expander(char **s, t_data *shell, char *var)
{
	char	*str;
	char	*expand;
	char	*new;
	int		i;
	int		k;

	str = *s;
	expand = NULL;
	new = create_buffer(str, shell, var, &expand);
	if (new == NULL)
		return ;
	i = -1;
	while (&str[++i] != var - 1)
		new[i] = str[i];
	k = i;
	while (expand && *expand)
		new[i++] = *expand++;
	if (expand && *var == '?')
		free(expand - (i - k));
	k += get_varlen(var) + 1;
	while (str[k])
		new[i++] = str[k++];
	free(str);
	*s = new;
}

//function that deletes the $s not expandibles(outside of quotes)
void	del_dollar(t_token *lst)
{
	char	*q;
	int		i;

	while (lst)
	{
		i = 0;
		while (lst->str[i] && ft_strchr(&lst->str[i], '$'))
		{
			q = &lst->str[i];
			if (is_type(QUOTE, &lst->str[i]))
				while (lst->str[++i])
					if (lst->str[i] == *q)
						break ;
			if (lst->str[i++] == '$')
			{
				if (lst->str[i] != '\0')
				{
					q = &lst->str[i - 1];
					while (*q)
						*q++ = lst->str[i++];
				}
			}
		}
		lst = lst->next;
	}
}
