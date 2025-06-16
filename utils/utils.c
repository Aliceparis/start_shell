/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yujin <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 14:38:19 by yujin             #+#    #+#             */
/*   Updated: 2025/06/14 14:38:20 by yujin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	error_message(t_shell *shell_program, const char *msg, int exit_s)
{
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
	shell_program->exit_status = exit_s;
}

int	count_env_vars(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		count++;
		env = env->next;
	}
	return (count);
}

char	**env_list_to_array(t_env *env)
{
	int		count;
	t_env	*tmp;
	char	**array;
	char	*entry;

	count = count_env_vars(env);
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

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}
