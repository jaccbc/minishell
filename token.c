#include "includes/minishell.h"
#include "libft/libft.h"


// limpar o array proveniente do split
void	free_array(char **arr)
{
	int		i;

	i = 0;
	while(arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

//avalia baseado no conteúdo do token, de que tipo se trata
int token_recognition(char *s)
{
	int	i;

	i = 0;
    if (!s || !*s)
        return TOKEN_UNKNOWN;
    if (ft_strncmp(s, "|", ft_strlen(s)) == 0)
        return TOKEN_PIPE;
    if (ft_strncmp(s, ">", ft_strlen(s)) == 0)
        return TOKEN_REDIRECTION_OUT;
    if (ft_strncmp(s, "<", ft_strlen(s)) == 0)
        return TOKEN_REDIRECTION_IN;
    if (ft_strncmp(s, ">>", ft_strlen(s)) == 0)
        return TOKEN_APPEND_OUT;
    if (s[0] == '-' && ft_strlen(s) > 1) 
	{
        while(s[++i])
		{
            if (!ft_isalnum(s[i]))
                return TOKEN_UNKNOWN;
        }
        return TOKEN_FLAG;
    }
    return TOKEN_COMMAND;
}

//cria um token novo no formato node, outros atributos serão colocados conforme vamos precisando
token_list	*ft_new_token(char *s, int type)
{
	token_list	*new_node;

	new_node = malloc(sizeof(token_list));
	if (!new_node)
		return (NULL);
	new_node->str = s;
	new_node->type = type;
	new_node->next = NULL;
	new_node->prev = NULL;
	return (new_node);
}

//adiciona o node ao fim da lista de tokens
void	ft_lstadd_token(token_list **lst, token_list *new)
{
	token_list	*current;

	if (!lst || !new)
		return ;
	current = *lst;
	if (current)
	{
		while (current->next != NULL)
		{
			current = current->next;
		}
		current->next = new;
		new->prev = current;
	}
	else
		*lst = new;
}

//só para sair o tipo e não o int que representa o tipo
char	*get_type(int type)
{
	char	*s_type;

	if (type == 1)
		s_type = "Command";
	else if (type == 2)
		s_type = "Flag";
	else if (type == 3)
		s_type = "Pipe";
	else if (type == 4)
		s_type = "Redirection out";
	return (s_type);
}

//auxiliar para imprimir a lista
void	print_list(token_list *lst)
{
	token_list	*current;
	int	i;

	current = lst;
	i = 0;
	while (current != NULL)
	{
		printf("arg nº%d: %s	tipo: %s\n", ++i, current->str, get_type(current->type));
		current = current->next;
	}
}

//limpa a lista
int		free_lst(token_list **lst)
{
	token_list	*current;
	token_list	*temp;

	current = *lst;
	while(current != NULL)
	{
		temp = current;
		current = current->next;
		free (temp);
	}
	*lst = NULL;
	return (0);
}

//convergência
int		tokenization(char *s)
{
	char	**arr;
	int		i;
	mini_data	data = {0};
	token_list	*token;

	arr = ft_split(s, ' ');
	i = -1;
	while(arr[++i] != NULL)
	{
		token = ft_new_token(arr[i], token_recognition(arr[i]));
		ft_lstadd_token(&data.token, token);
	}
	print_list(data.token);
	free_lst(&data.token);
	free_array(arr);
	return (0);
}

/* int	main(int argc, char **argv)
{
	if (argc == 2)
	{
		tokenization(argv[1]);
	}
} */