/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vamachad <vamachad@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 21:19:10 by vamachad          #+#    #+#             */
/*   Updated: 2024/06/25 21:23:24 by vamachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*buff_incomplete(int fd, char *buf, t_gnl_list **lst)
{
	int		bytes;
	char	*line;

	line = NULL;
	bytes = read(fd, buf, BUFFER_SIZE);
	while (bytes > 0)
	{
		buf[bytes] = '\0';
		node_buf(lst, buf, bytes);
		if (ft_strchr_gnl(buf, '\n'))
			break ;
		bytes = read(fd, buf, BUFFER_SIZE);
	}
	if (bytes <= 0 && !*lst)
		return (free_list(lst));
	line = get_line(*lst, line);
	sort_buf(buf);
	free_list(lst);
	return (line);
}

char	*buff_exists(char *buf, t_gnl_list **lst)
{
	char	*line;
	int		i;

	line = NULL;
	i = 0;
	while (buf[i])
	{
		if (buf[i] == '\n')
		{
			node_buf(lst, buf, ft_strlen_gnl(buf));
			line = get_line(*lst, line);
			sort_buf(buf);
			free_list(lst);
			return (line);
		}
		i++;
	}
	if (buf[--i])
	{
		node_buf(lst, buf, ft_strlen_gnl(buf));
	}
	return (NULL);
}

int	ft_strlen_gnl(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int	ft_strchr_gnl(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*free_list(t_gnl_list **lst)
{
	t_gnl_list	*temp;

	while (*lst)
	{
		temp = (*lst)->next;
		free(*lst);
		*lst = temp;
	}
	return (NULL);
}
