/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmen <jmen@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 19:31:05 by yujin             #+#    #+#             */
/*   Updated: 2025/06/10 15:12:32 by jmen             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_key_env(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '=')
			return (clean_old_content(ft_substr(str, 0, i), false));
		i++;
	}
	return (ft_strdup(str));
}

int	ft_env(t_shell *shell_program, t_env *env)
{
	(void)shell_program;
	while (env)
	{
		printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
	return (0);
}

t_env	*envlist_new(char *key, char *value)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->key = clean_old_content(ft_strdup(key), false);
	if (value)
		new->value = clean_old_content(ft_strdup(value), false);
	new->next = NULL;
	return (new);
}

void	ft_envlist_addback(t_env **envlist, t_env *new)
{
	t_env	*current;

	if (!*envlist)
	{
		*envlist = new;
		return ;
	}
	current = *envlist;
	while (current && current->next)
		current = current->next;
	current->next = new;
}
