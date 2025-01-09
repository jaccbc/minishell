/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sighandler.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 19:04:31 by joandre-          #+#    #+#             */
/*   Updated: 2025/01/09 14:23:11 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	g_signal;

static void	signl_prompt(int signum)
{
	g_signal = signum;
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
	act_nl.sa_flags = SA_RESTART;
	act_ign.sa_flags = SA_RESTART;
	act_nl.sa_handler = signl_prompt;
	act_ign.sa_handler = SIG_IGN;
	if (sigaction(SIGINT, &act_nl, NULL) == -1
		|| sigaction(SIGQUIT, &act_ign, NULL) == -1)
		return (false);
	return (true);
}

static void	signal_nonint(int signal)
{
	if (signal == SIGINT)
	{
		g_signal = signal;
		ft_putchar_fd('\n', STDOUT_FILENO);
	}
}

void	sighandler_noninteractive(void)
{
	struct sigaction	act;

	ft_memset(&act, 0, sizeof(act));
	act.sa_handler = &signal_nonint;
	if (sigaction(SIGINT, &act, NULL) == -1
		|| sigaction(SIGQUIT, &act, NULL) == -1)
		perror("signals");
}
