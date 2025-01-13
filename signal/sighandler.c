/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sighandler.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vamachad <vamachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 19:04:31 by joandre-          #+#    #+#             */
/*   Updated: 2025/01/13 17:11:15 by vamachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	g_signal;

static void	signl_prompt(int signum)
{
	g_signal = signum;
	if (signum == SIGINT)
	{
		ft_putchar_fd('\n', STDOUT_FILENO);
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
		return (perror("signals"), false);
	act_nl.sa_flags = 0;
	act_ign.sa_flags = 0;
	act_nl.sa_handler = signl_prompt;
	act_ign.sa_handler = SIG_IGN;
	if (sigaction(SIGINT, &act_nl, NULL) == -1
		|| sigaction(SIGQUIT, &act_ign, NULL) == -1)
		return (perror("signals"), false);
	return (true);
}

static void	signal_nonint(int signal)
{
	if (signal == SIGINT)
	{
		g_signal = signal;
		ft_putchar_fd('\n', STDIN_FILENO);
	}
}

void	sighandler_noninteractive(void)
{
	struct sigaction	act;

	signal(SIGQUIT, SIG_IGN);
	ft_memset(&act, 0, sizeof(act));
	act.sa_handler = &signal_nonint;
	if (sigaction(SIGINT, &act, NULL) == -1)
		perror("signals");
}
