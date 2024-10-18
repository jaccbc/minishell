/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vamachad <vamachad@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 21:54:14 by vamachad          #+#    #+#             */
/*   Updated: 2024/10/18 02:50:47 by vamachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	line_length(t_gnl_list *lst)
{
	int			len;
	t_gnl_list	*current;
	int			i;

	current = lst;
	len = 0;
	while (current)
	{
		i = 0;
		while (current->line[i] && current->line[i] != '\n')
		{
			i++;
			len++;
		}
		if (current->line[i] == '\n')
		{
			len++;
			break ;
		}
		current = current->next;
	}
	return (len);
}

void	node_buf(t_gnl_list **lst, char *buf, int size)
{
	t_gnl_list	*new_node;
	int			i;
	t_gnl_list	*current;

	new_node = (t_gnl_list *)malloc(sizeof(t_gnl_list));
	if (!new_node)
		return ;
	i = 0;
	while (buf[i] && i < size)
	{
		new_node->line[i] = buf[i];
		i++;
	}
	new_node->line[size] = '\0';
	new_node->next = NULL;
	current = *lst;
	if (current)
	{
		while (current->next != NULL)
			current = current->next;
		current->next = new_node;
	}
	else
		*lst = new_node;
	current = *lst;
}

char	*get_line(t_gnl_list *lst, char *line)
{
	int	i;
	int	j;

	i = 0;
	line = malloc(line_length(lst) + 1);
	if (!line)
	{
		free_list(&lst);
		return (NULL);
	}
	while (lst)
	{
		j = 0;
		while (lst->line[j] && lst->line[j] != '\n')
			line[i++] = lst->line[j++];
		if (lst->line[j] == '\n')
		{
			line[i++] = '\n';
			break ;
		}
		lst = lst->next;
	}
	line[i] = '\0';
	return (line);
}

void	sort_buf(char *buf)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (buf[i] && buf[i] != '\n')
		i++;
	if (buf[i] == '\n')
		++i;
	while (buf[i])
	{
		buf[j] = buf[i];
		i++;
		j++;
	}
	while (buf[j])
		buf[j++] = '\0';
}

char	*get_next_line(int fd)
{
	static char	buf[FOPEN_MAX][BUFFER_SIZE + 1];
	t_gnl_list	*lst;
	char		*line;

	lst = NULL;
	line = NULL;
	if (fd < 0 || BUFFER_SIZE <= 0 || fd > FOPEN_MAX)
		return (NULL);
	line = buff_exists(buf[fd], &lst);
	if (line)
		return (line);
	return (buff_incomplete(fd, buf[fd], &lst));
}
