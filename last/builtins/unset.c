#include "../include/minishell.h"

t_env *ft_unset(t_env *env, const char *key)
{
    t_env *current = env;
    t_env *prev = NULL;

    while (current)
    {
        if (ft_strcmp(current->key, key) == 0)
        {
            if (prev)
                prev->next = current->next;
            else
                env = current->next;
            /*free(current->key);
            free(current->value);
            free(current);*/
            return (env);
        }
        prev = current;
        current = current->next;
    }
    printf("unset; '%s' not fount in env\n", key);
    return (env);
}
