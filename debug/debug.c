/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 22:10:33 by joandre-          #+#    #+#             */
/*   Updated: 2024/10/06 02:46:52 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Print command list for debugging, including pipe output status
void print_command_list(t_command *cmd_list)
{
    t_command *current;
    int i;

    current = cmd_list;
    i = 0;
    printf("*** DEBUG PRINT_COMMAND_LIST ***\n");
    while (current != NULL)
    {
        printf("COMMAND NODE Nº%d\n[%p]\n", ++i, (void *)current);
        printf("command=[%s]\n", current->command ? current->command : "(null)");

        // Print arguments
        printf("args=[");
        if (current->args)
        {
            for (int j = 0; current->args[j] != NULL; j++)
                printf("\"%s\" ", current->args[j]);
        }
        else
            printf("(null)");
        printf("]\n");

        // Print the command path safely
        printf("path=[%s]\n", current->path ? current->path : "(null)");

        // Print infile redirection
        printf("infile=[");
        if (current->rdio->infile)
            printf("{name=\"%s\"}\n", current->rdio->infile);
        else
            printf("(null)");
        printf("]\n");

        // Print outfile redirection with type
        printf("outfile=[");
        if (current->rdio->outfile)
        {
            const char *type_str = (current->rdio->trn_or_app == REDIR_OUT_TRUNC) ? "truncate" : "append";
            printf("{name=\"%s\", type=[%s]} ", current->rdio->outfile, type_str);
        }
        else
            printf("(null)");
        printf("]\n");

        printf("pipe_fd[0]=[%d]\n", current->pipe_fd[0]);
        printf("pipe_fd[1]=[%d]\n", current->pipe_fd[1]);
        printf("has_pipe_output=[%s]\n", current->has_pipe_output ? "true" : "false");

        // Print linked list pointers
        printf("prev=[%p]\n", (void *)current->prev);
        printf("next=[%p]\n\n", (void *)current->next);

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
	printf("*** DEBUG PRINT_LIST ***\n");
	while (current != NULL)
	{
		printf("TOKEN Nº%d\n[%p]\n", ++i, current);
		printf("str=[%s]\ntype=[%s]\nprev=[%p]\nnext=[%p]\n\n", current->str,
			token_name(current->type), current->prev, current->next);
		current = current->next;
	}
	printf("*** DEBUG PRINT_LIST END***\n");
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

//retorna o nome do token_type
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
