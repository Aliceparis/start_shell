#include "minishell.h"

t_env    envlist_new(char *key, char *value)
{
    t_env   *new;

    new = malloc(sizeof(t_env));
    if (!new)
        return (NULL);
    new->key = key;
    new->value = value;
    new->next = NULL;
    return (new);
}

void   ft_envlist_addback(t_env *list)
{
    t_env   *current;

    if (!shell_program.envlist)
    {
        shell_program.envlist = list;
        return ;
    }
    current = shell_program.envlist;
    while (current && current->next)
        current = current->next;
    current->next = list;
}
