/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_value.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmen <jmen@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 13:35:18 by yujin             #+#    #+#             */
/*   Updated: 2025/06/14 20:49:14 by jmen             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		value = resultat;
	}
	return (resultat);
}

void	update_token_value(t_token_type type, char *value, t_token **token_list)
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
