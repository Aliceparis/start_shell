#include "libft.h"
#include "minishell.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

t_env *ft_unset(t_env *env, const char *key)
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