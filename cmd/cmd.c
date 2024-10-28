/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vamachad <vamachad@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 20:05:55 by vamachad          #+#    #+#             */
/*   Updated: 2024/10/28 20:09:46 by vamachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Create and initialize an empty command node
static t_command	*create_empty_command(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	ft_memset(cmd, 0, sizeof(t_command));
	cmd->pipe_fd[0] = -1;
	cmd->pipe_fd[1] = -1;
	cmd->has_pipe_output = false;
	cmd->rdio = malloc(sizeof(t_redirect));
	if (!cmd->rdio)
		return (free(cmd), NULL);
	cmd->rdio->infile = NULL;
	cmd->rdio->outfile = NULL;
	cmd->rdio->fd_in = -1;
	cmd->rdio->fd_out = -1;
	return (cmd);
}

// Set redirection target based on token type, storing only the last entries
void	handle_redirection(t_redirect *rdio, t_token *token)
{
	if (token->type == RED_IN)
	{
		if (rdio->infile)
			free(rdio->infile);
		rdio->infile = ft_strdup(token->next->str);
	}
	else if (token->type == RED_OUT)
	{
		if (rdio->outfile)
			free(rdio->outfile);
		rdio->outfile = ft_strdup(token->next->str);
		rdio->trn_or_app = REDIR_OUT_TRUNC;
	}
	else if (token->type == APPEND)
	{
		if (rdio->outfile)
			free(rdio->outfile);
		rdio->outfile = ft_strdup(token->next->str);
		rdio->trn_or_app = REDIR_OUT_APPEND;
	}
}

// Process token data
static bool	process_token_data(t_token *token, t_command *cmd, t_data *shell)
{
	if (token->type == COMMAND || (token->type == ARG && !cmd->command))
	{
		if (!get_cmd(cmd, token, shell))
			return (false);
	}
	else if (token->type == ARG)
		get_arg(cmd, token);
	else if (is_redirection(token))
		handle_redirection(cmd->rdio, token);
	return (true);
}

// Add a command node to the back of the command list
static void	add_command_back(t_command **head, t_command *new)
{
	t_command	*last;

	if (!head || !new)
		return ;
	if (!*head)
		*head = new;
	else
	{
		last = *head;
		while (last->next)
			last = last->next;
		last->next = new;
		new->prev = last;
	}
}

// Parse tokens and construct a command list within shell 
bool	final_parse(t_data *shell)
{
	t_command	*cmd;
	t_token		*token;

	shell->command = create_empty_command();
	cmd = shell->command;
	token = shell->lst;
	while (token)
	{
		if (token->type == PIPE)
		{
			cmd->has_pipe_output = true;
			add_command_back(&shell->command, create_empty_command());
			cmd = cmd->next;
		}
		else
		{
			if (!process_token_data(token, cmd, shell))
				return (false);
			if (is_redirection(token))
				token = token->next;
		}
		if (token)
			token = token->next;
	}
	return (true);
}
