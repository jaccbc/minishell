/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vamachad <vamachad@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 16:53:29 by vamachad          #+#    #+#             */
/*   Updated: 2024/10/06 16:54:15 by vamachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	is_delimiter(char c)
{
	return (c == ' ' || c == '|' || c == '>' || c == '<');
}

void	*ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
	void	*new_ptr;

	new_ptr = malloc(new_size);
	if (!new_ptr)
		return (NULL);
	if (ptr)
	{
		ft_memcpy(new_ptr, ptr, old_size);
		free(ptr);
	}
	return (new_ptr);
}

void	init_split_state(t_split_state *state)
{
	state->j = 0;
	state->k = 0;
	state->array_size = 10;
	state->in_quotes = false;
	state->quote_char = '\0';
}

char	**add_to_array(char **arr, char *buf, t_split_state *state)
{
	size_t	old_size;

	arr[state->k] = ft_strdup(buf);
	state->k++;
	if (state->k >= state->array_size)
	{
		old_size = state->array_size * sizeof(char *);
		state->array_size *= 2;
		arr = ft_realloc(arr, old_size, state->array_size * sizeof(char *));
	}
	return (arr);
}

void	handle_quote(char **s, t_split_state *state, char *buf)
{
	state->in_quotes = !state->in_quotes;
	if (state->in_quotes)
		state->quote_char = **s;
	buf[state->j] = **s;
	state->j++;
	(*s)++;
}

void	handle_delimiter(char **s, char ***arr, char *buf, t_split_state *state)
{
	if (state->j > 0)
	{
		buf[state->j] = '\0';
		*arr = add_to_array(*arr, buf, state);
		state->j = 0;
	}
	if (**s == '|')
		*arr = add_to_array(*arr, "|", state);
	else if (**s == '>')
	{
		if (*(*s + 1) == '>')
		{
			*arr = add_to_array(*arr, ">>", state);
			(*s)++;
		}
		else
			*arr = add_to_array(*arr, ">", state);
	}
	else if (**s == '<')
		*arr = add_to_array(*arr, "<", state);
	(*s)++;
}

char	**split_args(char *s)
{
	char			buf[10000];
	char			**arr;
	t_split_state	state;

	init_split_state(&state);
	arr = malloc(state.array_size * sizeof(char *));
	while (*s)
	{
		if ((*s == '\'' || *s == '\"') && (!state.in_quotes
				|| *s == state.quote_char))
			handle_quote(&s, &state, buf);
		else if (is_delimiter(*s) && !state.in_quotes)
			handle_delimiter(&s, &arr, buf, &state);
		else
			buf[state.j++] = *(s++);
	}
	if (state.j > 0)
	{
		buf[state.j] = '\0';
		arr = add_to_array(arr, buf, &state);
	}
	arr[state.k] = NULL;
	return (arr);
}

/* int	main(void)
{
	char	**tokens;
	int		i;
	char	*command;

	command = "ls -la|grep \"ola\" | wc -l";
	tokens = split_args(command);
	i = 0;
	while (tokens[i])
	{
		printf("Token[%d]: %s\n", i, tokens[i]);
		free(tokens[i]);
		i++;
	}
	free(tokens);
	return (0);
}
 */
