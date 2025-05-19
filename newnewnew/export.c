#include "libft.h"
#include "minishell.h"
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
        if (ft_strcmp(current->key, key) == 0)
        {
            current->value = ft_strdup(value);
            return (env);
        }
        current = current->next;
    }
    new_node = (t_env *)malloc(sizeof(t_env));
    if (!new_node)
        return (env);
    new_node->key = ft_strdup(key);
    new_node->value = ft_strdup(value);
    new_node->next = env;
    return (new_node);
}