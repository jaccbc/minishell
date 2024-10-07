/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dmain.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 22:44:26 by joandre-          #+#    #+#             */
/*   Updated: 2024/10/07 02:44:08 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//TOKENIZE
int	main(void)
{
	t_data	shell;
	char	*s;

	s = "echo \"Hello \'liltle $USER\'. Welcome to your $HOME\" >/dev/null";
	shell.lst = tokenize(s);
	if (!shell.lst)
		return (1);
	print_list(shell.lst);
	free_tokens(shell.lst);
	return (0);
}
