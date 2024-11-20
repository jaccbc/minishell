/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_files.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vamachad <vamachad@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 20:00:19 by vamachad          #+#    #+#             */
/*   Updated: 2024/11/11 02:38:10 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Function to output error messages in a Bash-like format
char	*minishell_errmsg(char *filename, char *error_message, bool prt_mini)
{
	char	*str1;
	char	*str2;

	if (prt_mini)
	{	
		str1 = ft_strjoin("minishell: ", filename);
		if (str1 == NULL)
			return (NULL);
	}
	else
	{
		str1 = ft_strdup(filename);
		if (str1 == NULL)
			return (NULL);
	}
	str2 = ft_strjoin(str1, ": ");
	free(str1);
	if (str2 == NULL)
		return (NULL);
	str1 = ft_strjoin(str2, error_message);
	return (free(str2), str1);
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
			cmd->error = minishell_errmsg(token->next->str, strerror(errno), true);
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
			(*cmd)->error = minishell_errmsg(token->next->str, strerror(errno), true);
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
