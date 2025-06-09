/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yujin <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 19:30:39 by yujin             #+#    #+#             */
/*   Updated: 2025/06/07 19:30:40 by yujin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_env	*ft_unset(t_env *env, const char *key)
{
	t_env	*current;
	t_env	*prev;

	current = env;
	prev = NULL;
	if (!env || !key)
		return (env);
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				env = current->next;
			return (env);
		}
		prev = current;
		current = current->next;
	}
	printf("unset; '%s' not fount in env\n", key);
	return (env);
}
