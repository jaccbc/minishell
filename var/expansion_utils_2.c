/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vamachad <vamachad@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 03:17:46 by vamachad          #+#    #+#             */
/*   Updated: 2024/10/18 03:17:47 by vamachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Comprimento da variável incluindo o $ ($USER = 5)
int	var_length(char *str)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i] && str[i] != '$')
		i++;
	if (str[i] == '$')
		count++;
	i++;
	if ((str[i] >= '0' && str[i] <= '9') || str[i] == '?')
		return (1);
	while (str[i] && is_var_compliant(str[i]))
	{
		i++;
		count++;
	}
	return (count);
}

// Extrai o nome da variável a seguir ao $ para que a possamos procurar em env
char	*extract_var_name(char *s)
{
	char	*start;
	char	*var;
	char	*temp;

	while (*s && *s != '$')
		s++;
	if (*s == '\0')
		return (NULL);
	start = ++s;
	while (*s && is_var_compliant(*s))
		s++;
	var = ft_substr(start, 0, s - start);
	if (!var)
		return (NULL);
	temp = var;
	var = ft_strjoin(temp, "=");
	free(temp);
	return (var);
}

// Apaga a variável. É usada no caso do value retornado por get_value ser nulo
int	erase_var(t_token *lst, int index)
{
	int		len;
	int		i;
	int		j;
	char	*new_str;

	i = 0;
	j = 0;
	len = var_length(&lst->str[index]);
	new_str = malloc(ft_strlen(lst->str) - len + 1);
	if (!new_str)
		return (1);
	while (lst->str[i])
	{
		if (i == index && lst->str[i] == '$')
			while (is_var_compliant(lst->str[++i]))
				;
		else
			new_str[j++] = lst->str[i++];
	}
	new_str[j] = '\0';
	free(lst->str);
	lst->str = new_str;
	return (0);
}

// Auxiliar para copiar o valor para a new_str em erase_and_replace
void	copy_value(char *new_str, int *j, char *value)
{
	int	k;

	k = 0;
	while (value[k])
		new_str[(*j)++] = value[k++];
}

// Quando encontramos a variável em env vamos substituir o valor retornado
// por get_value pela variável em si $USER -> vamachad
int	erase_and_replace_var(t_token *lst, int index, char *value)
{
	int		len;
	int		i;
	int		j;
	char	*new_str;

	i = 0;
	j = 0;
	len = var_length(&lst->str[index]);
	new_str = malloc(ft_strlen(lst->str) - len + ft_strlen(value) + 1);
	if (!new_str)
		return (1);
	while (lst->str[i])
	{
		if (i == index && lst->str[i] == '$')
		{
			copy_value(new_str, &j, value);
			i += len;
		}
		else
			new_str[j++] = lst->str[i++];
	}
	new_str[j] = '\0';
	free(lst->str);
	lst->str = new_str;
	return (0);
}
