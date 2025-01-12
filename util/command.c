/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 01:20:35 by joandre-          #+#    #+#             */
/*   Updated: 2024/12/08 02:32:31 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Add a command node to the back of the command list
void	add_command_back(t_command **head, t_command *new)
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

// Add argument to the t_command struct's args field
bool	fill_args(t_command **command, t_token *token)
{
	t_command	*cmd;
	int			i;

	i = 0;
	if (!command || !token)
		return (false);
	cmd = *command;
	if (cmd->args)
		while (cmd->args[i])
			i++;
	cmd->args = ft_realloc(cmd->args, i + 2);
	if (cmd->args == NULL)
		return (false);
	cmd->args[i] = ft_strdup(token->str);
	if (cmd->args[i] == NULL)
		return (false);
	cmd->args[i + 1] = NULL;
	return (true);
}

static void	del_redirect(t_redirect *rdio)
{
	if (rdio)
	{
		if (rdio->heredoc)
			unlink(rdio->infile);
		free(rdio->infile);
		free(rdio->outfile);
		if (rdio->fd_in != -1)
			close(rdio->fd_in);
		if (rdio->fd_out != -1)
			close(rdio->fd_out);
		if (rdio->stdin_backup != -1)
			close(rdio->stdin_backup);
		if (rdio->stdout_backup != -1)
			close(rdio->stdout_backup);
		free(rdio);
	}
}

void	lstdel_command(t_command *lst)
{
	t_command	*del;
	int			i;

	while (lst)
	{
		if (lst->command)
			if (lst->command[0] == '\0')
				free(lst->command);
		free(lst->error);
		if (lst->args)
		{
			i = -1;
			while (lst->args[++i])
				free(lst->args[i]);
			free(lst->args);
		}
		free(lst->path);
		del_redirect(lst->rdio);
		del = lst;
		lst = lst->next;
		free(del);
	}
}

t_redirect	*create_redirect(void)
{
	t_redirect	*new;

	new = ft_calloc(1, sizeof(t_redirect));
	if (new == NULL)
		return (NULL);
	new->infile = NULL;
	new->outfile = NULL;
	new->fd_in = -1;
	new->fd_out = -1;
	new->stdin_backup = -1;
	new->stdout_backup = -1;
	return (new);
}
