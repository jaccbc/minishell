/* ************************************************************************** */

#include "../minishell.h"

//retorna o token numero i da lst
//DOWN 0 == last_token | UP 0 == first_token
t_token	*lstiter_token(t_token *lst, int type, size_t i)
{
	if (!lst)
		return (NULL);
	if (i == 0)
		i = ULLONG_MAX;
	if (type == DOWN)
	{
		while (lst->next && i--)
			lst = lst->next;
	}
	else if (type == UP)
	{
		while (lst->prev && i--)
			lst = lst->prev;
	}
	return (lst);
}

//adiciona um token na lista
void	lstadd_token(t_token **lst, t_token *new)
{
	t_token	*current;

	if (!lst || !new)
		return ;
	current = *lst;
	if (current)
	{
		while (current->next)
			current = current->next;
		current->next = new;
		new->prev = current;
	}
	else
		*lst = new;
}

void	free_token(t_token *lst)
{
	t_token	*temp;

	while (lst != NULL)
	{
		temp = lst;
		lst = lst->next;
		free(temp->str);
		free(temp);
	}
}

bool	is_type(int type, char *s)
{
	char	*str;

	if (!s)
		return (false);
	if (type == UNQUOTE)
		return (*s == ' ' || *s == '|' || *s == '<' || *s == '>');
	if (type == DELIMIT)
		return (*s == '>' || *s == '<' || *s == '|');
	if (type == QUOTE)
	{
		str = s + 1;
		while (*str)
			if (*str++ == *s)
				return (true);
	}
	return (false);
}

bool	err_msg(char *msg, char *detail, bool in_quotes)
{
	char	*err_msg;

	err_msg = ft_strjoin("minishell: ", msg);
	if (in_quotes)
		err_msg = ft_strjoin(err_msg, "`");
	err_msg = ft_strjoin(err_msg, detail);
	if (in_quotes)
		err_msg = ft_strjoin(err_msg, "'");
	ft_putendl_fd(err_msg, STDERR_FILENO);
	free (err_msg);
	return (false);
}

bool	consecutive_sep(t_token *lst)
{
	t_token	*current;

	current = lst;
	while(current)
	{
		if (current->prev && current->prev->type > PIPE && current->type >= PIPE)
			return (err_msg("syntax error near unexpected token ", current->str, true));
		else if (current->type > PIPE && !current->next)
			return (err_msg("syntax error near unexpected token ", "newline", true));
		current = current->next;
	}
	return (false);
}

bool quotes_n_var(t_token *lst)
{
    t_token *current;
    bool s_quotes = false;
    bool d_quotes = false;

    current = lst;
    while (current)
    {
        int i = 0;
        while (current->str[i])
        {
            if (current->str[i] == '\"' && !s_quotes)
                d_quotes = !d_quotes;
            else if (current->str[i] == '\'' && !d_quotes)
                s_quotes = !s_quotes;
            if (current->str[i] == '$' && d_quotes)
                current->var_in_quotes = true;
            i++;
        }
        current = current->next;
    }
    return (s_quotes || d_quotes);
}


bool check_syntax(t_token *lst)
{
	t_token	*current;

	current = lst;
	if (current->type == HEREDOC && current->next)
		return (true);
	if (current->type == PIPE)
		return(err_msg("syntax error near unexpected token ", current->str, true));
	if (consecutive_sep(current))
		return(false);
	//if(quotes_n_var)
		//replicar comportamento quando não se fecha aspas
	return (true);
}
