/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 01:20:35 by joandre-          #+#    #+#             */
/*   Updated: 2024/11/11 02:54:18 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**ft_realloc(char **array, size_t new_size)
{
	char	**new_array;
	size_t	i;

	new_array = malloc(sizeof(char *) * new_size);
	if (!new_array)
		return (NULL);
	i = 0;
	while (array && array[i] && i < new_size - 1)
	{
		new_array[i] = array[i];
		i++;
	}
	new_array[i] = NULL;
	free(array);
	return (new_array);
}

// Add a command node to the back of the command list
void	add_command_back(t_command **head, t_command *new)
{
	t_command	*last;

	if (!head || !new)
		return ;
	if (!*head)
		*head = new;
	else
	{
		last = *head;
		while (last->next)
			last = last->next;
		last->next = new;
		new->prev = last;
	}
}

static bool	fill_data(t_token *token, t_command *cmd)
{
	if (access(token->str, F_OK) != -1 && access(token->str, X_OK) != -1)
	{
		cmd->path = ft_strdup(token->str);
		if (cmd->path == NULL)
			return (false);
		cmd->command = ft_strdup(ft_strrchr(cmd->path, '/') + 1);
		if (cmd->command == NULL)
			return (false);
	}
	else
	{
		cmd->error = minishell_errmsg(token->str, strerror(errno));
		return (false);
	}
	cmd->args = ft_realloc(cmd->args, 2);
	if (cmd->args == NULL)
		return (false);
	cmd->args[0] = cmd->command;
	cmd->args[1] = NULL;
	return (true);
}

static bool	is_directory(char *str, t_command **command)
{
	struct stat	data;
	int			i;
	bool		dir;

	if (!str || !(*str))
		return (false);
	dir = false;
	i = 0;
	while (str[i])
	{
		if (str[i++] == '/')
			dir = true;
	}
	if (dir && lstat(str, &data) == 0)
	{
		if (S_ISDIR(data.st_mode) && (*command)->error == NULL)
			(*command)->error = minishell_errmsg(str, "Is a directory");
		if (S_ISLNK(data.st_mode) && (*command)->error == NULL)
			(*command)->error = minishell_errmsg(str, "Permission denied");
		if (access(str, X_OK) == -1 && (*command)->error == NULL)
			(*command)->error = minishell_errmsg(str, strerror(errno));
	}
	if ((*command)->error)
		return (true);
	return (false);
}

// Get the command string and initialize args with command name
bool	fill_command(t_command **command, t_token *token, t_data *shell)
{
	if (!command || !(*command) || !token)
		return (false);
	if (is_directory(token->str, command))
		return (false);
	if (is_type(PATH, token->str))
		return (fill_data(token, *command));
	else
		(*command)->command = ft_strdup(token->str);
	if ((*command)->command == NULL)
		return (false);
	(*command)->args = ft_realloc((*command)->args, 2);
	if ((*command)->args == NULL)
		return (false);
	(*command)->args[0] = (*command)->command;
	(*command)->args[1] = NULL;
	if ((*command)->path)
		return (true);
	fill_command_path((*command), shell);
	if (!(*command)->path)
	{
		(*command)->error = minishell_errmsg((*command)->command,
				"command not found");
		return (false);
	}
	return (true);
}

// Add argument to the t_command struct's args field
bool	fill_args(t_command **command, t_token *token)
{
	t_command	*cmd;
	int			i;

	i = 0;
	if (!command || !token)
		return (false);
	cmd = *command;
	if (cmd->args)
		while (cmd->args[i])
			i++;
	cmd->args = ft_realloc(cmd->args, i + 2);
	if (cmd->args == NULL)
		return (false);
	cmd->args[i] = ft_strdup(token->str);
	if (cmd->args[i] == NULL)
		return (false);
	cmd->args[i + 1] = NULL;
	return (true);
}

static void	del_redirect(t_redirect *rdio)
{
	if (rdio)
	{
		if (rdio->heredoc)
			unlink(rdio->infile);
		free(rdio->infile);
		free(rdio->outfile);
		free(rdio);
	}
}

void	lstdel_command(t_command *lst)
{
	t_command	*del;
	int			i;

	while (lst)
	{
		if (lst->error)
			free(lst->error);
		if (lst->args)
		{
			i = -1;
			while (lst->args[++i])
				free(lst->args[i]);
			free(lst->args);
		}
		if (lst->path)
			free(lst->path);
		del_redirect(lst->rdio);
		del = lst;
		lst = lst->next;
		free(del);
	}
}
