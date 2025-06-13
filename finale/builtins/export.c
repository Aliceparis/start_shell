/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmen <jmen@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 19:30:09 by yujin             #+#    #+#             */
/*   Updated: 2025/06/10 14:46:20 by jmen             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_env	*ft_export(t_env *env, const char *key, const char *value)
{
	t_env	*current;
	t_env	*new_node;

	current = env;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			current->value = clean_old_content(ft_strdup(value), false);
			return (env);
		}
		current = current->next;
	}
	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (env);
	new_node->key = clean_old_content(ft_strdup(key), false);
	new_node->value = clean_old_content(ft_strdup(value), false);
	ft_envlist_addback(&env, new_node);
	new_node->next = NULL;
	return (env);
}
