/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 19:46:24 by joandre-          #+#    #+#             */
/*   Updated: 2024/11/02 05:43:20 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	var_lookup(char **line, char **env)
{
	char	*str;
	int		i;

	if (!line || !(*line))
		return ;
	str = *line;
	i = 0;
	while (str[i])
	{
		if (is_type(VAR, &str[i++]))
		{
			if (ft_isalnum(str[i]) || str[i] == '_' || str[i] == '?')
			{
				expander(line, env, &str[i]);
				i = 0;
				continue ;
			}
		}
	}
}

static bool	get_doc(char *delimiter, t_redirect *rdio, char **env)
{
	char	*line;

	if (!delimiter || !rdio || !env)
		return (free(delimiter), false);
	rdio->infile = ft_strdup(".temp_heredoc");
	rdio->fd_in = open(rdio->infile, O_CREAT | O_RDWR | O_TRUNC, 0664);
	if (rdio->fd_in < 0)
		return (free(delimiter), false);
	while (1)
	{
		ft_putstr_fd("> ", STDIN_FILENO);
		line = get_next_line(STDIN_FILENO);
		if (!line || !(*line))
			return (free(delimiter), false);
		var_lookup(&line, env);
		if (ft_strncmp(line, delimiter, ft_strlen(line)) == 0)
			break ;
		free(line);
	}
	close(rdio->fd_in);
	rdio->fd_in = -1;
	free(line);
	return (free(delimiter), true);
}

bool	parse_heredoc(t_redirect *rdio, t_token *lst, char **env)
{
	if (!rdio || !lst)
		return (false);
	if (rdio->fd_in != -1)
		close(rdio->fd_in);
	if (get_doc(ft_strjoin(lst->str, "\n"), rdio, env) == false)
		return (false);
	rdio->heredoc = true;
	return (true);
}
