/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_files.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vamachad <vamachad@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 20:00:19 by vamachad          #+#    #+#             */
/*   Updated: 2024/12/04 13:17:31 by vamachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*join_strs(char *str, char *add)
{
	char	*tmp;

	if (!add)
		return (str);
	if (!str)
		return (ft_strdup(add));
	tmp = str;
	str = ft_strjoin(tmp, add);
	free(tmp);
	return (str);
}

char	*mini_errmsg(char *command, char *detail, char *error_message,
		bool prt_mini)
{
	char	*msg;
	bool	detail_quotes;

	msg = NULL;
	detail_quotes = false;
	if (prt_mini)
		msg = ft_strdup("minishell: ");
	if (command != NULL)
	{
		msg = join_strs(msg, command);
		msg = join_strs(msg, ": ");
	}
	if (detail != NULL)
	{
		if (ft_strncmp(command, "export", 7) == 0)
			detail_quotes = true;
		if (detail_quotes)
			msg = join_strs(msg, "`");
		msg = join_strs(msg, detail);
		if (detail_quotes)
			msg = join_strs(msg, "'");
		msg = join_strs(msg, ": ");
	}
	msg = join_strs(msg, error_message);
	return (msg);
}

static void	write_heredoc(t_token *lst, t_command **cmd, char **env)
{
	while (lst && lst->type != PIPE)
	{
		if (lst->type == HEREDOC)
		{
			if ((*cmd)->rdio == NULL)
				(*cmd)->rdio = create_redirect();
			parse_heredoc((*cmd)->rdio, lst->next, env);
		}
		lst = lst->next;
	}
}

// Unified function to open a file
// handle error reporting with strerror(errno)
static bool	open_file(t_command *cmd, t_token *token, int flags, int mode)
{
	if (cmd->rdio == NULL)
		cmd->rdio = create_redirect();
	if (cmd->rdio->fd_out != -1)
		close(cmd->rdio->fd_out);
	cmd->rdio->fd_out = open(token->next->str, flags, mode);
	if (cmd->rdio->fd_out == -1)
	{
		if (cmd->error == NULL)
			cmd->error = mini_errmsg(token->next->str, NULL, strerror(errno),
					true);
		g_last_exit_code = 1;
		return (false);
	}
	return (true);
}

// Verify all redirection files for access errors
bool	check_files(t_token *token, t_command **cmd, char **env)
{
	write_heredoc(token, cmd, env);
	while (token && token->type != PIPE)
	{
		if (token->type == RED_IN && access(token->next->str, R_OK) != 0)
		{
			(*cmd)->error = mini_errmsg(token->next->str, NULL, strerror(errno),
					true);
			g_last_exit_code = 1;
			return (false);
		}
		else if (token->type == APPEND)
		{
			if (!open_file(*cmd, token, O_WRONLY | O_CREAT | O_APPEND, 0644))
				return (false);
		}
		else if (token->type == RED_OUT)
		{
			if (!open_file(*cmd, token, O_WRONLY | O_CREAT | O_TRUNC, 0644))
				return (false);
		}
		token = token->next;
	}
	return (true);
}
