/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dmain.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 22:44:26 by joandre-          #+#    #+#             */
/*   Updated: 2024/10/05 23:29:08 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//TOKENIZE
int	main(void)
{
	t_data	shell;
	char	*prompt = "echo $HOME >> /path/to/file";

	shell.lst = tokenize(prompt);
	if (!shell.lst)
		return (1);
	print_list(shell.lst);
	free_tokens(&shell.lst);
	return (0);
}
