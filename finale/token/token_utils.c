#include "minishell.h"

/*ft_token:
	1. skip espaces
	2. check the double quote and get the value between the quote
	3. check the token type, and get the token type seperator
	4. get others information, is the word token type
	5. update the type and the value ine the structure*/
int	ft_isspace(char c)
{
	if ((c >= 9 && c <= 13) || c == ' ')
		return (1);
	return (0);
}

void	skip_space(char **input)
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

/*在token中，是否保留引号？？
echo "Je m'appelle $firstName, j'ai ${age} ans."
# Output : Je m'appelle Alex, j'ai 30 ans.*/
char	*get_between_quote(char **input)
{
	char		*resultat;
	const char	*start;
	char		quote;
	int			len;

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
