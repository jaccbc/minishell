/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 01:34:20 by joandre-          #+#    #+#             */
/*   Updated: 2024/11/01 01:23:35 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//função inicia syntax analysis
bool	check_syntax(t_data *shell)
{
	if (!shell->lst)
		return (false);
	if (!syntax_error(shell->lst))
		return (false);
	var_expander(shell);
	del_dollar(shell->lst);
	if (!del_quote(shell->lst))
		return (false);
	if (!check_files(shell->lst))
		return (false);
	return (true);
}
