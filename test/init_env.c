#include "minishell.h"


void    init_envlist(void)
{
    char    **environ;
    int i;
    char    *key;
    char    *value;

    environ = shell_program.environ;
    if (!environ)
        return ;
    i = 0;
    while (environ[i])
    {
        key = get_key_env(environ[i]);
        value = get_value_env(environ[i]);
        update_envlist(key, value);
        i++;
    }
}

char    *get_key_env(char *str)
{
    int i;

    i = 0;
    while (str[i])
    {
        if (str[i] == '=')
            return (ft_substr(str, 0, i));
        i++;
    }
    return (ft_strdup(str));
}

char    *get_value_env(char *str)
{
    int i;

    i = 0;
    while (str[i])
    {
        if (str[i] == '=')
        {
            i++;
            return (ft_substr(str, i, ft_strlen(str) - i));
        }
        i++;
    }
    return (NULL);
}
void    update_envlist(char *key, char *value)
{
    t_env   *envlist;

    envlist = shell_program.envlist;
    if (envlist)
    {
        if (!ft_strncmp(key, envlist->key, ft_strlen(key)))
        {
            envlist->value = value;
            return ;
        }
        envlist = envlist->next;
    }
    else
        ft_envlist_addback(envlist_new(key, value));
}