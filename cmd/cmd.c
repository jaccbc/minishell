/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vamachad <vamachad@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 20:05:55 by vamachad          #+#    #+#             */
/*   Updated: 2024/12/09 19:46:35 by joandre-         ###   ########.fr       */
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
	new->error = NULL;
	new->args = NULL;
	new->path = NULL;
	new->pipe_fd = NULL;
	new->rdio = NULL;
	new->next = NULL;
	new->prev = NULL;
	new->error = NULL;
	new->has_pipe_output = false;
	return (new);
}

static bool	have_heredoc(t_token *token)
{
	while (token && token->type != PIPE)
	{
		if (token->type == HEREDOC)
			return (true);
		token = token->next;
	}
	return (false);
}

static bool	handle_redirection(t_redirect *rdio, t_token *token)
{
	char	*filename;

	filename = ft_strdup(token->next->str);
	if (filename == NULL)
		return (false);
	if (token->type == RED_IN && !have_heredoc(token))
	{
		if (rdio->heredoc)
		{
			unlink(rdio->infile);
			rdio->heredoc = false;
		}
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
static void	process_token_data(t_token *token, t_command *cmd, t_data *shell)
{
	while (token && token->type != PIPE)
	{
		if (token->type == COMMAND && token->str
			&& !(*token->str) && token->next)
			token = token->next;
		if (token->type == COMMAND || (token->type == ARG && !cmd->command
				&& token->prev && !(token->prev->type >= RED_IN)))
			fill_command(&cmd, token, shell);
		else if (token->type == ARG)
			fill_args(&cmd, token);
		else if (token->type >= RED_IN)
		{
			if (cmd->rdio == NULL)
				cmd->rdio = create_redirect();
			if (token->type != HEREDOC)
				handle_redirection(cmd->rdio, token);
			token = token->next;
		}
		if (token)
			token = token->next;
	}
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
			if (cmd->next == NULL)
				return (false);
			cmd = cmd->next;
			token = token->next;
		}
		if (check_files(token, &cmd, shell->env))
			process_token_data(token, cmd, shell);
		while (token && token->type != PIPE)
			token = token->next;
	}
	return (lstdel_token(shell->lst), true);
}
