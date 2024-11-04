/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 19:46:24 by joandre-          #+#    #+#             */
/*   Updated: 2024/11/04 04:25:57 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	null_errmsg(int i, char *delimiter)
{
	ft_putstr_fd("minishell: warning: here-document at line ", STDERR_FILENO);
	ft_putnbr_fd(i, STDERR_FILENO);
	ft_putstr_fd(" delimited by end-of-file (wanted `", STDERR_FILENO);
	write(STDERR_FILENO, delimiter, ft_strlen(delimiter) - 1);
	ft_putstr_fd("')\n", STDERR_FILENO);
}

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

static void	get_user_input(t_redirect *rdio, char *delimiter, char **env)
{
	static int	i = 0;
	char		*line;

	while (1)
	{
		line = readline("> ");
		if (line == NULL)
		{
			null_errmsg(i, delimiter);
			break ;
		}
		var_lookup(&line, env);
		if (*line && ft_strncmp(line, delimiter, ft_strlen(line)) == 0)
			break ;
		ft_putstr_fd(line, rdio->fd_in);
		free(line);
		++i;
	}
	free(line);
	i = 0;
}

static bool	fill_heredoc(char *delimiter, t_redirect *rdio, char **env)
{
	if (!delimiter || !rdio || !env)
		return (free(delimiter), false);
	rdio->infile = ft_strdup(".temp_heredoc");
	rdio->fd_in = open(rdio->infile, O_CREAT | O_RDWR | O_TRUNC, 0664);
	if (rdio->fd_in < 0)
		return (free(delimiter), false);
	get_user_input(rdio, delimiter, env);
	close(rdio->fd_in);
	rdio->fd_in = -1;
	return (free(delimiter), true);
}

bool	parse_heredoc(t_redirect *rdio, t_token *lst, char **env)
{
	if (!rdio || !lst)
		return (false);
	if (rdio->fd_in != -1)
		close(rdio->fd_in);
	if (fill_heredoc(ft_strjoin(lst->str, "\n"), rdio, env) == false)
		return (false);
	rdio->heredoc = true;
	return (true);
}
