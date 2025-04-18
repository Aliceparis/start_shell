/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loulou <loulou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 13:58:58 by loulou            #+#    #+#             */
/*   Updated: 2025/04/18 15:22:19 by loulou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "../minishell.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

t_env *unset(t_env *env, const char *key)
{
    t_env *current = env;
    t_env *prev = NULL;

    while (current)
    {
        //找key
        if (ft_strcmp(current->key, key) == 0)
        {
            //如不是第一个在list里 就next
            if (prev)
                prev->next = current->next;
            //如是第一个 就往后移
            else
                env = current->next;
            free(current->key);
            free(current->value);
            free(current);
            return (env);
        }
        prev = current;
        current = current->next;
    }
    return (env);
}
