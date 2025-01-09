/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 19:46:24 by joandre-          #+#    #+#             */
/*   Updated: 2025/01/09 14:42:18 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	null_errmsg(char *delimiter)
{
	ft_putstr_fd("minishell: warning: here-document", STDERR_FILENO);
	ft_putstr_fd(" delimited by end-of-file (wanted `", STDERR_FILENO);
	write(STDERR_FILENO, delimiter, ft_strlen(delimiter));
	ft_putstr_fd("')\n", STDERR_FILENO);
}

static char	*var_lookup(t_token *lst, char *line, t_data *shell)
{
	char	*str;
	int		i;

	str = ft_strdup(line);
	if (str == NULL)
		return (free(line), NULL);
	if (lst->d_quotes && lst->s_quotes)
		return (free(line), str);
	i = 0;
	while (str[i])
	{
		if (is_type(VAR, &str[i++]))
		{
			if (ft_isalnum(str[i]) || str[i] == '_' || str[i] == '?')
			{
				expander(&str, shell, &str[i]);
				i = 0;
				continue ;
			}
		}
	}
	return (free(line), str);
}

static bool	get_user_input(t_token *lst, t_redirect *rdio, t_data *shell)
{
	char		*line;
	char		*str;

	str = NULL;
	while (1)
	{
		ft_putstr_fd("> ", STDOUT_FILENO);
		line = get_next_line(STDIN_FILENO);
		if (g_signal == SIGINT)
		{
			g_signal = 0;
			return (free(line), false);
		}
		if (line == NULL)
			return (null_errmsg(lst->str), false);
		str = var_lookup(lst, line, shell);
		if (*str && str[0] != '\n' && ft_strlen(str) - 1 == ft_strlen(lst->str)
			&& ft_strncmp(str, lst->str, ft_strlen(str) - 1) == 0)
			break ;
		ft_putstr_fd(str, rdio->fd_in);
		free(str);
	}
	return (free(str), true);
}

static void	generate_name(t_redirect *rdio)
{
	char	*name;

	name = ft_itoa((long long)rdio);
	if (name == NULL)
		return ;
	rdio->infile = ft_strjoin(".temp_heredoc", name);
	free(name);
}

bool	parse_heredoc(t_redirect *rdio, t_token *lst, t_data *shell)
{
	if (!rdio || !lst)
		return (false);
	if (rdio->fd_in != -1)
		close(rdio->fd_in);
	if (rdio->infile && !rdio->heredoc)
		free(rdio->infile);
	if (rdio->heredoc == false)
	{
		generate_name(rdio);
		if (rdio->infile == NULL)
			return (false);
	}
	if (rdio->infile == NULL)
		return (false);
	rdio->fd_in = open(rdio->infile, O_CREAT | O_RDWR | O_TRUNC, 0664);
	if (rdio->fd_in == -1)
		return (false);
	get_user_input(lst, rdio, shell);
	close(rdio->fd_in);
	rdio->fd_in = -1;
	rdio->heredoc = true;
	return (true);
}
