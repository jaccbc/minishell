/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vamachad <vamachad@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 20:06:05 by vamachad          #+#    #+#             */
/*   Updated: 2024/10/28 20:06:07 by vamachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Get the command string and initialize args with command name
bool	get_cmd(t_command *cmd, t_token *token, t_data *shell)
{
	if (!cmd || !token)
		return (false);
	cmd->command = ft_strdup(token->str);
	cmd->args = ft_realloc(cmd->args, 2);
	cmd->args[0] = cmd->command;
	cmd->args[1] = NULL;
	fill_command_path(cmd, shell);
	if (!cmd->path)
	{
		err_msg_basic(cmd->command, "command not found");
		return (false);
	}
	return (true);
}

// Add argument to the t_command struct's args field
void	get_arg(t_command *cmd, t_token *token)
{
	int	i;

	i = 0;
	if (!cmd || !token)
		return ;
	if (cmd->args)
		while (cmd->args[i])
			i++;
	cmd->args = ft_realloc(cmd->args, i + 2);
	if (!cmd->args)
		return ;
	cmd->args[i] = ft_strdup(token->str);
	cmd->args[i + 1] = NULL;
}

// Determine if the token is a redirection
bool	is_redirection(t_token *token)
{
	return (token->type == RED_IN || token->type == RED_OUT
		|| token->type == APPEND);
}
