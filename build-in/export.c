/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loulou <loulou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 13:58:57 by loulou            #+#    #+#             */
/*   Updated: 2025/04/22 15:44:28 by loulou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "../minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

t_env *ft_export(t_env *env, const char *key, const char *value)
{
    t_env *current;
    t_env *new_node;

    current = env;
    while (current)
    {
        //如找到就替换并free前一个value
        if (ft_strcmp(current->key, key) == 0)
        {
            free(current->value);
            current->value = ft_strdup(value);
            return (env);
        }
        current = current->next;
    }
    //不然新造一个
    new_node = (t_env *)malloc(sizeof(t_env));
    if (!new_node)
        return (env);
    new_node->key = ft_strdup(key);
    new_node->value = ft_strdup(value);
    new_node->next = env;
    return (new_node);
}
