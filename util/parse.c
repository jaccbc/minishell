/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vamachad <vamachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 17:29:24 by vamachad          #+#    #+#             */
/*   Updated: 2025/01/13 17:33:03 by vamachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	init_parse(t_data *shell, t_command *cmd)
{
	cmd->has_pipe_output = true;
	add_command_back(&shell->command, create_command());
	if (cmd->next == NULL)
		return (false);
	return (true);
}
