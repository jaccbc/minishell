/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 19:51:51 by joandre-          #+#    #+#             */
/*   Updated: 2024/10/12 01:12:47 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static bool	valid_print(t_token *lst)
{
	if (!lst)
		return (false);
	return (lst->type == ARG || lst->type == VAR || lst->type == FLAG);
}

static bool	verify_cmd(t_token **lst, bool *nl)
{
	if (!lst || *lst == NULL)
		return (false);
	if ((*lst)->type != COMMAND)
		return (false);
	if (ft_strncmp((*lst)->str, "echo", ft_strlen((*lst)->str)))
		return (false);
	*lst = (*lst)->next;
	if (*lst == NULL)
		return (true);
	if ((*lst)->type == FLAG
		&& !ft_strncmp((*lst)->str, "-n", ft_strlen((*lst)->str)))
	{
		*nl = false;
		*lst = (*lst)->next;
	}
	return (true);
}

int	echo(t_token *lst)
{
	bool	nl;

	if (!lst)
		return (1);
	nl = true;
	if (!verify_cmd(&lst, &nl))
		return (1);
	if (valid_print(lst))
		ft_putstr_fd(lst->str, STDOUT_FILENO);
	lst = lst->next;
	while (valid_print(lst))
	{
		ft_putstr_fd(" ", STDOUT_FILENO);
		ft_putstr_fd(lst->str, STDOUT_FILENO);
		lst = lst->next;
	}
	if (nl)
		write(STDOUT_FILENO, "\n", 1);
	return (0);
}
