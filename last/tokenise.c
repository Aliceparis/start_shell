# include "minishell.h"

/*ft_token:
    1. skip espaces
    2. check the double quote and get the value between the quote
    3. check the token type, and get the token type seperator
    4. get others information, is the word token type
    5. update the type and the value ine the structure*/
int ft_isspace(char c )
    {
        if ((c >= 9 && c <= 13) || c == ' ')
            return (1);
    
        return (0);
    }
    
void    skip_space(char **input)
    {
        while (**input && ft_isspace(**input))
            (*input)++;
    }
int	is_quote(char c)
    {
        if (c == '"' || c == '\'')
            return (1);
        return (0);
    }
int	is_quote_close(char *str)
    {
        char	quote;
    
        quote = 0;
        while (*str)
        {
            if (*str == '"' || *str == '\'')
            {
                if (!quote)
                    quote = *str;
                else if (quote == *str)
                    quote = 0;
            }
            str++;
        }
        if (!quote)
            return (1);
        return (0);
    }
    /*在token中，是否保留引号？？*/
    /*echo "Je m'appelle $firstName, j'ai ${age} ans."
    # Output : Je m'appelle Alex, j'ai 30 ans.*/
char    *get_token_between_quote(char **input)
    {
        char    *resultat;
        const char    *start;
        char	quote;
        int len;
    
        resultat = NULL;
        quote = **input;
        start = *input;
        (*input)++;
        len = 1;
        while (**input && **input != quote)
        {
            (*input)++;
            len++;
        }
        if (**input == quote)
        {
            (*input)++;
            len++;
        }
        resultat = clean_old_content(ft_substr(start, 0, len), false);
        return (resultat);
    }
    
int is_token_operator(char *input)
    {
        if (!ft_strncmp(input, ">>", 2) || !ft_strncmp(input, "<<", 2) || *input == '>' || *input == '<' || *input == '|'
            )
            return (1);
        return (0);
    }
char    *get_token_operator(char **input)
    {
        char    *resultat;
    
        resultat = NULL;
        if (!ft_strncmp(*input, "<<", 2) || !ft_strncmp(*input, ">>", 2))
        {
            resultat = clean_old_content(ft_substr(*input, 0, 2), false);
            *input = *input + 2; 
        }
        else if (**input == '<' || **input == '>' || **input == '|')
        {
            resultat = clean_old_content(ft_substr(*input, 0, 1), false);
            (*input)++;
        }
        return (resultat);
    }

char    *get_token_word(char **input)
    {
        char    *start;
        char    *resultat;
        int len;
    
        start = *input;
        len = 0;
        resultat = NULL;
        while (**input && (!ft_isspace(**input)) && !is_token_operator(*input) && !is_quote(**input))
        {
            (*input)++;
            len++;
        }
        resultat = clean_old_content(ft_substr(start, 0, len), false);
        return (resultat);
    }
    
void    ft_token(char   *line, t_shell *shell_program)
{
    char    *value;
    
    value = NULL;
    if (!is_quote_close(line))
    {
        /////fonction de quite et free, (error : error syntax)??????
		error_message(shell_program, "Error: unmatched quote found in input.", 1);
		free_token(&(shell_program->token_list));
        reset_terminal(shell_program);
        return ;
    }
    while (*line)
    {
        skip_space(&line);
        if (!(*line))
        {
            reset_terminal(shell_program);
            return ;
        }
        if (is_quote(*line))
            value = get_token_between_quote(&line); 
        else if (is_token_operator(line))
            value = get_token_operator(&line);
        else
            value = get_token_word(&line);
		if (value)
			update_token_value(get_type_of_token(value), value, &shell_program->token_list);
		//free(value);
    }
}


token_type    get_type_of_token(char *str)
{
        if (!ft_strncmp(str, ">>", 2))
            return (REDICT_APPEND);
        if (!ft_strncmp(str, "<<", 2))
            return (IS_HEREDOC);
        if (*str == '|')
            return (IS_PIPE);
        if (*str == '>')
            return (REDICT_OUT);
        if (*str == '<')
            return (REDICT_IN);
        return (IS_WORD);
}
    
void update_token_value(token_type type, char *value, t_token **token_list)
{
    t_token *new;
    t_token *tmp;

    new = malloc(sizeof(t_token));
    if (!new)
        return ;
    new->type = type;
    new->value = clean_old_content(ft_strdup(value), false);
    new->next = NULL;
    if (!*token_list)
        *token_list = clean_old_content(new, false);
    else
    {
        tmp = *token_list;
		while (tmp->next)
			tmp = tmp->next;
        tmp->next = clean_old_content(new, false);
	}
}
