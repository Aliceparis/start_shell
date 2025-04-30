#include "minishell.h"

void    init_envlist(t_shell *shell_program)
{
    char    **environ;
    int i;
    char    *key;
    char    *value;

    environ = shell_program->environ;
	key = NULL;
	value = NULL;
    if (!environ)
        return ;
    i = 0;
    while (environ[i])
    {
		if (!key)
        	key = get_key_env(environ[i]);
		else
		{
			free(key);
			key = get_key_env(environ[i]);
		}
		if (!value)
	        value = get_value_env(environ[i]);
		else
		{
			free(value);
			value = get_key_env(environ[i]);
		}
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
            return (ft_substr(str, 0, i));
        i++;
    }
    return (ft_strdup(str));
}

char    *get_value_env(char *str)
{
    int i;
	char	*resultat;

    i = 0;
	resultat = NULL;
    while (str[i])
    {
        if (str[i] == '=')
        {
            i++;
            resultat = ft_substr(str, i, ft_strlen(str) - i);
        }
        i++;
    }
    return (resultat);
}
void    update_envlist(t_shell *shell_program, char *key, char *value)
{
    t_env   *envlist;

	envlist = shell_program->envlist;
    while (envlist)
    {
        if (envlist->key && ft_strcmp(envlist->key, key) == 0)
        {
			free(envlist->value);
            envlist->value = ft_strdup(value);
			free(key);
			free(value);
            return ;
        }
        envlist = envlist->next;
    }
    if (!envlist)
		ft_envlist_addback(&(shell_program->envlist), envlist_new(key, value));
}
