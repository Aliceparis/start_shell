#include "minishell.h"

void    init_envlist(t_shell *shell_program)
{
    char    **environ;
    int i;
    char    *key;
    char    *value;

    environ = shell_program->environ;
    if (!environ)
        return ;
    i = 0;
    while (environ[i])
    {
        key = get_key_env(environ[i]);
        value = get_value_env(environ[i]);
        update_envlist(shell_program, key, value);
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
            return (clean_old_content(ft_substr(str, 0, i), false));
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
            return (clean_old_content(ft_substr(str, i, ft_strlen(str) - i), false));
        }
        i++;
    }
    return (NULL);
}
void    update_envlist(t_shell *shell_program, char *key, char *value)
{
    t_env   *envlist;

    envlist = shell_program->envlist;
    while (envlist)
    {
        if (!ft_strncmp(key, envlist->key, ft_strlen(key)))
        {
			if (value)
            	envlist->value = clean_old_content(ft_strdup(value), false);
            return ;
        }
        envlist = envlist->next;
    }
    if (!envlist)
        ft_envlist_addback(&(shell_program->envlist), envlist_new(key, value));
}
static void ft_del(void *ptr)
{
    free(ptr);
    ptr = NULL;
}
void    *clean_old_content(void *ptr, bool clean)
{
    static t_list   *old_content;

    if (clean)
    {
        ft_lstclear(&old_content, ft_del);
        return (NULL);
    }
    else
    {
        ft_lstadd_back(&old_content, ft_lstnew(ptr));
        return (ptr);
    }
}
