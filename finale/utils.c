#include "minishell.h"

void error_message(t_shell *shell_program, const char *msg, int exit_s)
{
    write(2, msg,ft_strlen(msg));
	write(2, "\n", 1);
    shell_program->exit_status = exit_s;
}

char **env_list_to_array(t_env *env)
{
	int		count;
	t_env	*tmp;
	char	**array;
	char	*entry;

    count = 0;
    tmp = env;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	array = clean_old_content(malloc(sizeof(char *) * (count + 1)), false);
	if (!array)
		return (NULL);
	tmp = env;
	count = 0;
	while (tmp)
	{
		entry = ft_strjoin(tmp->key, "=");
		array[count] = clean_old_content(ft_strjoin(entry, tmp->value), false);
		free(entry);
		count++;
		tmp = tmp->next;
	}
	array[count] = NULL;
	return (array);
}
