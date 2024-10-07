/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dmain.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 22:44:26 by joandre-          #+#    #+#             */
/*   Updated: 2024/10/06 19:33:38 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//TOKENIZE
int	main(void)
{
	t_data	shell;

	shell.lst = tokenize("echo \"Hello \'liltle $USER\'. Welcome to your $HOME\"");
	if (!shell.lst)
		return (1);
	print_list(shell.lst);
	free_tokens(shell.lst);
	return (0);
}
