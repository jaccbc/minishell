/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sighandler.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 19:04:31 by joandre-          #+#    #+#             */
/*   Updated: 2024/11/03 01:00:24 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	signl_prompt(int signum)
{
	if (signum == SIGINT)
	{
		ft_putchar_fd('\n', STDIN_FILENO);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

bool	sighandler(void)
{
	struct sigaction	act_nl;
	struct sigaction	act_ign;

	if (sigemptyset(&act_nl.sa_mask) == -1
		|| sigemptyset(&act_ign.sa_mask) == -1)
		return (false);
	act_nl.sa_flags = 0;
	act_ign.sa_flags = 0;
	act_nl.sa_handler = signl_prompt;
	act_ign.sa_handler = SIG_IGN;
	if (sigaction(SIGINT, &act_nl, NULL) == -1
		|| sigaction(SIGQUIT, &act_ign, NULL) == -1)
		return (false);
	return (true);
}
