/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vamachad <vamachad@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 20:06:05 by vamachad          #+#    #+#             */
/*   Updated: 2024/11/02 02:43:12 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Get the command string and initialize args with command name
bool	fill_command(t_command **command, t_token *token, t_data *shell)
{
	t_command	*cmd;

	if (!command || !token)
		return (false);
	cmd = *command;
	cmd->command = ft_strdup(token->str);
	if (cmd->command == NULL)
		return (false);
	cmd->args = ft_realloc(cmd->args, 2);
	if (cmd->args == NULL)
		return (false);
	cmd->args[0] = cmd->command;
	cmd->args[1] = NULL;
	fill_command_path(cmd, shell);
	if (!cmd->path)
		return (minishell_errmsg(cmd->command, "command not found"), false);
	return (true);
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

void	lstdel_command(t_command *lst)
{
	t_command	*del;
	int			i;

	while (lst)
	{
		if (lst->args)
		{
			i = -1;
			while (lst->args[++i])
				free(lst->args[i]);
			free(lst->args);
		}
		free(lst->path);
		if (lst->rdio)
		{
			if (lst->rdio->heredoc)
				unlink(lst->rdio->infile);
			free(lst->rdio->infile);
			free(lst->rdio->outfile);
			free(lst->rdio);
		}
		del = lst;
		lst = lst->next;
		free(del);
	}
}
