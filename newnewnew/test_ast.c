#include "tokenising.h"
#include "parsing.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}
char	*ft_strdup(const char *s)
{
	char	*resultat;
	int		i;

	resultat = (char *)malloc(sizeof(char) * (ft_strlen(s) + 1));
	if (resultat == NULL)
		return (NULL);
	i = 0;
	while (s[i])
	{
		resultat[i] = s[i];
		i++;
	}
	resultat[i] = '\0';
	return (resultat);
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
char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char			*new;
	unsigned int	i;

	if (start >= ft_strlen(s))
		len = 0;
	else if (len > ft_strlen(s + start))
		len = ft_strlen(s + start);
	new = (char *)malloc(sizeof(char) * (len + 1));
	if (!new)
		return (NULL);
	i = 0;
	while (start + i < ft_strlen(s) && i < len)
	{
		new[i] = s[start + i];
		i++;
	}
	new[i] = '\0';
	return (new);
}
int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	if (n == 0)
		return (0);
	while (s1[i] && i < n - 1 && s2[i] && (s1[i] == s2[i]))
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}
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
	if (c == '\'' || c == '"')
		return (1);
	return (0);
}
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
    resultat = ft_substr(start, 0, len);
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
        resultat = ft_substr(*input, 0, 2);
        *input = *input + 2; 
    }
    else if (**input == '<' || **input == '>' || **input == '|')
    {
        resultat = ft_substr(*input, 0, 1);
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
    while (**input && (!ft_isspace(**input)) && !is_token_operator(*input) && **input != '"')
    {
        (*input)++;
        len++;
    }
    resultat = ft_substr(start, 0, len);
    return (resultat);
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
    new->value = value;
    new->next = NULL;
    if (!*token_list)
        *token_list = new;
    else
    {
        tmp = *token_list;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = new;
    }
}
t_token    *ft_token(char   *line)
{
    char    *value;
    t_token  *token_list;

    value = NULL;
    token_list = NULL;
	if (!is_quote_close(line))
	{
		/////fonction de quite et free, (error : error syntax)??????
        fprintf(stderr, "Error: unmatched quote found in input.\n");
        exit(EXIT_FAILURE);
    }
    while (*line)
    {
        skip_space(&line); 
        if (is_quote(*line))
            value = get_token_between_quote(&line); 
        else if (is_token_operator(line))
            value = get_token_operator(&line);
        else
            value = get_token_word(&line);
        update_token_value(get_type_of_token(value), value, &token_list);
    }
	return (token_list);
}
ASTnode	*simple_commande(t_token **token)
{
	t_token	*tmp;
	int	count_args;
	ASTnode	*node;
	char	**args;
	int	i;

	tmp = *token;
	count_args = 0;
	i = 0;
	while (tmp && tmp->type == IS_WORD)
	{
		count_args++;
		tmp = tmp->next;
	}
	args = malloc(sizeof(char *) * (count_args + 1));
	if (!args)
		////fonction error and free (error: malloc echoue)
		printf("mallor failled");
	while (*token && (*token)->type == IS_WORD)
	{
		args[i++] = ft_strdup((*token)->value);
		*token = (*token)->next;
	}
	args[i] = 0;
	node = malloc(sizeof(ASTnode));
	if (!node)
	////fonction error and free (error: malloc echoue)
	node->type = CMD;
	node->args = args;
	node->left = node->right = 0;
	return (node);
}

ASTnode	*ft_parse(t_token **token)
{
	ASTnode *left;
	ASTnode	*node;
	
	if (!token || !(*token)) //保护
		return (NULL);
	left = simple_commande(token);
	//printf("left type : %u, arguments0 : %s, arguments 1:%s\n", left->type, left->args[0], left->args[1]);
	while (*token)
	{
		if ((*token)->type == IS_PIPE)
		{
			node = malloc(sizeof(ASTnode));
			if (!node)
		////fonction error and free (error: malloc echoue)
			*token = (*token)->next;
			node->left = left;
			node->type = PIPE;
			node->right = ft_parse(token);
			left = node;
		}
		else if ((*token)->type == REDICT_OUT || (*token)->type == REDICT_IN || (*token)->type == IS_HEREDOC || (*token)->type == REDICT_APPEND)
		{
			node = malloc(sizeof(ASTnode));
			if (!node)
		////fonction error and free (error: malloc echoue)
			node->operator = (*token)->value;
			*token = (*token)->next;
			if (!(*token) && (*token)->type != IS_WORD)
				printf("token est null\n");////fonction error and free (error: expected filename or deliminiter)
			node->file = ft_strdup((*token)->value);
			*token = (*token)->next;
			node->type = REDIRECTION;
			node->left = left;
			node->right = ft_parse(token);
			left = node;
		}
		else
			break ;
	}
	return (left);
}

void print_ast(ASTnode *node, int depth)
{
    if (!node)
        return;
    for (int i = 0; i < depth; i++)
        printf("  ");
    printf("Node type: %d\n", node->type);  // 你也可以写成字符串形式

    if (node->args)
    {
        for (int i = 0; node->args[i]; i++)
        {
            for (int j = 0; j < depth; j++)
                printf("  ");
            printf("type:%u, Arg: %s\n", node->type, node->args[i]);
        }
    }

    if (node->file)
    {
        for (int j = 0; j < depth; j++)
            printf("  ");
        printf("File: %s\n", node->file);
    }

    print_ast(node->left, depth + 1);
    print_ast(node->right, depth + 1);
}
int main()
{
    char *test = "echo hello";

    t_token *token_list = ft_token(test);
    ASTnode *ast = ft_parse(&token_list);

   print_ast(ast, 0);

    return 0;
}