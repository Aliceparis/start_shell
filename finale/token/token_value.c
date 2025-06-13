#include "minishell.h"

char	*get_next_token(char **line)
{
	char	*value;
	char	*resultat;
	char	*tmp;

	value = ft_strdup("");
	resultat = NULL;
	while (**line && !ft_isspace(**line) && !is_token_operator(*line))
	{
		if (is_quote(**line))
			tmp = get_between_quote(line);
		else
			tmp = get_token_word(line);
		resultat = ft_strjoin(value, tmp);
		free(value);
		free(tmp);
	}
	return (resultat);
}

token_type	get_type_of_token(char *str)
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

void	update_token_value(token_type type, char *value, t_token **token_list)
{
	t_token	*new;
	t_token	*tmp;

	new = malloc(sizeof(t_token));
	if (!new)
		return (free(value), (void)0);
	new->type = type;
	new->value = ft_strdup(value);
	if (!new->value)
	{
		free(value);
		free(new);
		return ;
	}
	new->next = NULL;
	free(value);
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
