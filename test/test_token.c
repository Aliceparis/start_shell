#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
/*在token中，是否保留引号？？*/
/*token 里面，单双引号是否关闭已经作了检查，但是如果没有关闭的情况下，要显示error还未添加*/
#include "tokenising.h"
#include "parsing.h"
size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
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

void    ft_token(char   *line)
{
    char    *value;
    //t_token  *token_list;

    value = NULL;
    //token_list = NULL;
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
		if (value)
		{
			printf("[%s]\n", value);
			free(value);
		}
    }
}

/*int	main(void)
{
	char	*input = " echo \"oqnioa > test.txt";

	ft_token(input);
}*/