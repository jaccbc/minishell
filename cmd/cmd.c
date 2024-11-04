/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vamachad <vamachad@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 20:05:55 by vamachad          #+#    #+#             */
/*   Updated: 2024/11/04 04:05:05 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Create and initialize an empty command node
static t_command	*create_command(void)
{
	t_command	*new;

	new = ft_calloc(1, sizeof(t_command));
	if (new == NULL)
		return (NULL);
	new->command = NULL;
	new->args = NULL;
	new->path = NULL;
	new->pipe_fd[0] = -1;
	new->pipe_fd[1] = -1;
	new->rdio = NULL;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

static t_redirect	*create_redirect(void)
{
	t_redirect	*new;

	new = ft_calloc(1, sizeof(t_redirect));
	if (new == NULL)
		return (NULL);
	new->infile = NULL;
	new->outfile = NULL;
	new->fd_in = -1;
	new->fd_out = -1;
	return (new);
}

// Set redirection target based on token type, storing only the last entries
static bool	handle_redirection(t_redirect *rdio, t_token *token)
{
	char	*filename;

	if (!rdio || !token || !token->next)
		return (false);
	filename = ft_strdup(token->next->str);
	if (filename == NULL)
		return (false);
	if (token->type == RED_IN)
	{
		if (rdio->infile)
			free(rdio->infile);
		rdio->infile = filename;
	}
	if (token->type == RED_OUT || token->type == APPEND)
		if (rdio->outfile)
			free(rdio->outfile);
	if (token->type == RED_OUT)
		rdio->outfile = filename;
	else if (token->type == APPEND)
		rdio->outfile = filename;
	return (true);
}

// Process token data
static bool	process_token_data(t_token *token, t_command *cmd, t_data *shell)
{
	if (token->type == COMMAND || (token->type == ARG && !cmd->command))
		return (fill_command(&cmd, token, shell));
	else if (token->type == ARG)
		return (fill_args(&cmd, token));
	else if (token->type >= RED_IN)
	{
		if (cmd->rdio == NULL)
			cmd->rdio = create_redirect();
		if (token->type == HEREDOC)
			return (parse_heredoc(cmd->rdio, token->next, shell->env));
		return (handle_redirection(cmd->rdio, token));
	}
	return (false);
}

// Parse tokens and construct a command list within shell
bool	final_parse(t_data *shell)
{
	t_token		*token;
	t_command	*cmd;

	shell->command = create_command();
	if (shell->command == NULL)
		return (false);
	token = shell->lst;
	cmd = shell->command;
	while (token)
	{
		if (token->type == PIPE)
		{
			cmd->has_pipe_output = true;
			add_command_back(&shell->command, create_command());
			cmd = cmd->next;
		}
		else if (!process_token_data(token, cmd, shell))
			return (false);
		token = token->next;
	}
	return (lstdel_token(shell->lst), true);
}
