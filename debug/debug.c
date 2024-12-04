/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 22:10:33 by joandre-          #+#    #+#             */
/*   Updated: 2024/12/04 13:16:44 by vamachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_file_content(const char *filename)
{
	int		fd;
	char	*line;

	if (!filename)
		return ;
	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return ;
	printf("Content of file \"%s\":\n", filename);
	while ((line = get_next_line(fd)) != NULL)
	{
		printf("%s", line); // Print each line as read
		free(line);         // Free line after printing
	}
	close(fd);
	printf("\n"); // Add a newline after file content for readability
}

// Print command list for debugging, including pipe output status
void	print_command_list(t_command *cmd_list)
{
	t_command	*current;
	int			i;

	current = cmd_list;
	i = 0;
	printf("*** DEBUG PRINT_COMMAND_LIST ***\n");
	while (current != NULL)
	{
		printf("COMMAND NODE Nº%d\n[%p]\n", ++i, current);
		printf("command=[%s]\n", current->command);
		printf("args=[");
		if (current->args)
		{
			for (int j = 0; current->args[j] != NULL; j++)
				printf("\"%s\" ", current->args[j]);
		}
		else
			printf("(null)");
		printf("]\n");
		printf("error=[%s]\n", current->error);
		printf("path=[%s]\n", current->path);
		printf("has_pipe_output=[%s]\n",
			current->has_pipe_output ? "true" : "false");
		if (current->rdio)
		{
			printf("infile=[%s]\n", current->rdio->infile);
			print_file_content(current->rdio->infile);
			printf("outfile=[%s]\n", current->rdio->outfile);
			print_file_content(current->rdio->outfile);
			printf("heredoc=[%s]\n", current->rdio->heredoc ? "true" : "false");
			printf("fd_in=[%d]\n", current->rdio->fd_in);
			printf("fd_out=[%d]\n", current->rdio->fd_out);
		}
		printf("prev=[%p]\n", current->prev);
		printf("next=[%p]\n\n", current->next);
		current = current->next;
	}
	printf("*** DEBUG PRINT_COMMAND_LIST END ***\n");
}

// imprime a lista de tokens
void	print_list(t_token *lst)
{
	t_token	*current;
	int		i;

	current = lst;
	i = 0;
	printf("*** DEBUG PRINT_TOKEN_LIST ***\n");
	while (current != NULL)
	{
		printf("TOKEN Nº%d\n[%p]\n", ++i, current);
		printf("str=[%s]\ntype=[%s]\nprev=[%p]\nnext=[%p]\n\n", current->str,
			token_name(current->type), current->prev, current->next);
		current = current->next;
	}
	printf("*** DEBUG PRINT_TOKEN_LIST END***\n");
}

void	print_array(char **arr)
{
	int	i;

	i = -1;
	while (arr[++i])
	{
		printf("%s\n", arr[i]);
	}
}

// retorna o nome do token_type
char	*token_name(int type)
{
	if (type == COMMAND)
		return ("COMMAND");
	if (type == ARG)
		return ("ARG");
	if (type == RED_IN)
		return ("RED_IN");
	if (type == RED_OUT)
		return ("RED_OUT");
	if (type == PIPE)
		return ("PIPE");
	if (type == APPEND)
		return ("APPEND");
	if (type == HEREDOC)
		return ("HEREDOC");
	return ("INVALID");
}
