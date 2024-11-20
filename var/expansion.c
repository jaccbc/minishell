/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vamachad <vamachad@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 03:02:37 by vamachad          #+#    #+#             */
/*   Updated: 2024/11/06 03:05:53 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static size_t	get_varlen(const char *s)
{
	size_t	len;

	len = 0;
	while (ft_isalnum(s[len]) || s[len] == '_' || s[len] == '?')
		++len;
	return (len);
}

// Procura em env a variável e retorna o valor
// a funcao "ft_strchr(env[i], '=') + 1" retorna tudo que está apos = na **env
static char	*get_value(char **env, const char *s)
{
	int		i;
	char	*var;

	if (s[0] == '?')
		return (ft_strdup(ft_itoa(g_last_exit_code)));
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
	while (env[i] && ft_strncmp(var, env[i], ft_strlen(var)))
		++i;
	free(var);
	if (env[i] == NULL)
		return (NULL);
	return (ft_strchr(env[i], '=') + 1);
}

//mede o length das strings e alloca memoria para a nova string expandida
static void	*create_buffer(char *str, char **env, char *var, char **expand)
{
	size_t	len;

	len = 1;
	*expand = get_value(env, var);
	if (*expand)
		len += ft_strlen(*expand);
	len += ft_strlen(str) - get_varlen(var);
	return (ft_calloc(len, sizeof(char)));
}

//funcao interna responsável pela var expansion
//1º while loop copia tudo ate encontrar o $ da variavel
//2º while loop copia a expansao se existir
//3º while loop copia o resto da string
void	expander(char **s, char **env, char *var)
{
	char	*str;
	char	*expand;
	char	*new;
	int		i;
	int		k;

	str = *s;
	expand = NULL;
	new = create_buffer(str, env, var, &expand);
	if (new == NULL)
		return ;
	i = -1;
	while (&str[++i] != var - 1)
		new[i] = str[i];
	k = i;
	if (expand)
		while (*expand)
			new[i++] = *expand++;
	k += get_varlen(var) + 1;
	while (str[k])
		new[i++] = str[k++];
	free(str);
	*s = new;
}

//função responsável por apagar os dollars não expansiveis (fora de quotes)
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
