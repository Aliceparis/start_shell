/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmen <jmen@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 14:29:17 by yujin             #+#    #+#             */
/*   Updated: 2025/05/27 18:00:33 by jmen             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void ft_env(t_shell *shell_program, t_env *env)
{
    (void)shell_program;
    while (env)
    {
        printf("%s=%s\n", env->key, env->value);
        env = env->next;
    }
}

t_env    *envlist_new(char *key, char *value)
{
    t_env   *new;

    new = clean_old_content(malloc(sizeof(t_env)), false);
    if (!new)
        return (NULL);
    new->key = clean_old_content(ft_strdup(key), false);
	if (value)
    	new->value = clean_old_content(ft_strdup(value), false);
    new->next = NULL;
    return (new);
}

void   ft_envlist_addback(t_env **envlist, t_env *new)
{
    t_env   *current;

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