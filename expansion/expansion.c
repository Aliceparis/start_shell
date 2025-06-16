/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yujin <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 13:31:50 by yujin             #+#    #+#             */
/*   Updated: 2025/06/14 13:31:51 by yujin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_word(char *str, t_shell *shell_program)
{
	int		i;
	char	*tmp;
	char	*resultat;

	i = 0;
	resultat = clean_old_content(ft_strdup(""), false);
	while (str[i])
	{
		if (str[i] == '\'')
			tmp = content_in_single_quote(str, &i);
		else if (str[i] == '"')
			tmp = content_in_double_quote(str, &i, shell_program);
		else if (str[i] == '$')
			tmp = content_with_variable(str, &i, shell_program);
		else
			tmp = content_simple(str, &i);
		resultat = clean_old_content(ft_strjoin(resultat, tmp), false);
	}
	return (resultat);
}

char	*content_in_single_quote(char *str, int *i)
{
	int		start;
	char	*content;

	(*i)++;
	start = *i;
	while (str[*i] && str[*i] != '\'')
		(*i)++;
	content = clean_old_content(ft_substr(str, start, *i - start), false);
	if (str[*i] == '\'')
		(*i)++;
	return (content);
}

char	*content_in_double_quote(char *str, int *i, t_shell *shell_program)
{
	char	*tmp;
	char	*resultat;
	int		start;

	(*i)++;
	resultat = clean_old_content(ft_strdup(""), false);
	while (str[*i] && str[*i] != '"')
	{
		if (str[*i] == '$')
			tmp = content_with_variable(str, i, shell_program);
		else
		{
			start = *i;
			while (str[*i] && str[*i] != '"' && str[*i] != '$')
				(*i)++;
			tmp = clean_old_content(ft_substr(str, start, *i - start), false);
		}
		resultat = clean_old_content(ft_strjoin(resultat, tmp), false);
	}
	if (str[*i] == '"')
		(*i)++;
	return (resultat);
}

char	*getenv_value(t_shell *shell_program, const char *var_name)
{
	t_env	*envlist;

	envlist = shell_program->envlist;
	while (envlist)
	{
		if (ft_strcmp(envlist->key, var_name) == 0)
		{
			return (envlist->value);
		}
		envlist = envlist->next;
	}
	return (NULL);
}
