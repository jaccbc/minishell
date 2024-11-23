/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 19:46:24 by joandre-          #+#    #+#             */
/*   Updated: 2024/11/23 22:12:41 by joandre-         ###   ########.fr       */
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

static char	*var_lookup(t_token *lst, char *line, char **env)
{
	char	*str;
	int		i;

	str = ft_strdup(line);
	if (str == NULL)
		return (NULL);
	if (lst->d_quotes && lst->s_quotes)
		return (str);
	i = 0;
	while (str[i])
	{
		if (is_type(VAR, &str[i++]))
		{
			if (ft_isalnum(str[i]) || str[i] == '_' || str[i] == '?')
			{
				expander(&str, env, &str[i]);
				i = 0;
				continue ;
			}
		}
	}
	return (str);
}

static void	get_user_input(t_token *lst, t_redirect *rdio, char **env)
{
	char		*line;
	char		*str;

	str = NULL;
	while (1)
	{
		line = readline("> ");
		if (line == NULL)
		{
			null_errmsg(lst->str);
			return ;
		}
		str = var_lookup(lst, line, env);
		free(line);
		line = NULL;
		if (*str && ft_strcmp(str, lst->str))
			break ;
		ft_putendl_fd(str, rdio->fd_in);
		free(str);
	}
	free(str);
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

bool	parse_heredoc(t_redirect *rdio, t_token *lst, char **env)
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
	get_user_input(lst, rdio, env);
	close(rdio->fd_in);
	rdio->fd_in = -1;
	rdio->heredoc = true;
	return (true);
}
