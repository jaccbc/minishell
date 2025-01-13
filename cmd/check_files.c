/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_files.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vamachad <vamachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 20:00:19 by vamachad          #+#    #+#             */
/*   Updated: 2025/01/13 20:35:05 by vamachad         ###   ########.fr       */
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

static void	write_heredoc(t_token *lst, t_command **cmd, t_data *shell)
{
	shell->status = 0;
	while (lst && lst->type != PIPE)
	{
		if (lst->type == HEREDOC)
		{
			if ((*cmd)->rdio == NULL)
				(*cmd)->rdio = create_redirect();
			parse_heredoc((*cmd)->rdio, lst->next, shell);
			if (!g_signal && shell->status == 130)
				return ;
		}
		lst = lst->next;
	}
}

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
		return (false);
	}
	return (true);
}

bool	check_files(t_token *token, t_command **cmd, t_data *shell)
{
	write_heredoc(token, cmd, shell);
	if (shell->status == 130)
		return (false);
	while (token && token->type != PIPE)
	{
		if (token->type == RED_IN && access(token->next->str, R_OK) != 0)
		{
			(*cmd)->error = mini_errmsg(token->next->str, NULL, strerror(errno),
					true);
			shell->status = 1;
			return (false);
		}
		else if ((token->type == APPEND
				&& !open_file(*cmd, token, O_WRONLY | O_CREAT | O_APPEND, 0644))
			|| (token->type == RED_OUT
				&& !open_file(*cmd, token, O_WRONLY | O_CREAT | O_TRUNC, 0644)))
		{
			shell->status = 1;
			return (false);
		}
		token = token->next;
	}
	return (true);
}
