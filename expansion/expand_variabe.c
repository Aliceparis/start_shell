/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variabe.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yujin <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 13:31:32 by yujin             #+#    #+#             */
/*   Updated: 2025/06/14 13:31:33 by yujin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*extract_var_name(char *str, int *i)
{
	int	start;
	int	var_len;

	if (str[*i + 1] == '?')
	{
		*i += 2;
		return (ft_strdup("?"));
	}
	start = *i + 1;
	var_len = 0;
	while (ft_isalnum(str[start + var_len]) || str[start + var_len] == '_')
		var_len++;
	if (var_len == 0)
	{
		(*i)++;
		return (ft_strdup(""));
	}
	*i = start + var_len;
	return (ft_substr(str, start, var_len));
}

char	*content_with_variable(char *str, int *i, t_shell *shell_program)
{
	char	*var_name;
	char	*resultat;
	char	*tmp;

	var_name = extract_var_name(str, i);
	if (ft_strcmp(var_name, "?") == 0)
	{
		free(var_name);
		return (clean_old_content(ft_itoa(shell_program->exit_status), false));
	}
	if (ft_strlen(var_name) == 0)
	{
		free(var_name);
		return (ft_strdup("$"));
	}
	tmp = getenv_value(shell_program, var_name);
	free(var_name);
	resultat = clean_old_content(ft_strdup(""), false);
	if (tmp)
		resultat = clean_old_content(ft_strjoin(resultat, tmp), false);
	return (resultat);
}

char	*content_simple(char *str, int *i)
{
	int		j;
	char	*resultat;

	j = *i;
	while (str[*i] && str[*i] != '\'' && str[*i] != '"' && str[*i] != '$')
		(*i)++;
	resultat = clean_old_content(ft_substr(str, j, *i - j), false);
	return (resultat);
}
