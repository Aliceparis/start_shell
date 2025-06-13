#include "minishell.h"

int	is_token_operator(char *input)
{
	if (!ft_strncmp(input, ">>", 2) || !ft_strncmp(input, "<<", 2)
		|| *input == '>' || *input == '<' || *input == '|')
		return (1);
	return (0);
}

char	*get_token_operator(char **input)
{
	char	*resultat;

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

char	*get_token_word(char **input)
{
	char	*start;
	char	*resultat;
	int		len;

	start = *input;
	len = 0;
	resultat = NULL;
	while (**input && (!ft_isspace(**input)) && !is_token_operator(*input)
		&& !is_quote(**input))
	{
		(*input)++;
		len++;
	}
	resultat = ft_substr(start, 0, len);
	return (resultat);
}
