/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gestion_fds.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yujin <yujin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 11:49:57 by yujin             #+#    #+#             */
/*   Updated: 2025/06/14 15:06:33 by yujin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	close_and_restore(int original_fd, int std_fd)
{
	if (original_fd == -1)
		return (1);
	if (dup2(original_fd, std_fd) == -1)
	{
		perror("Erreur lors de la restauration du FD");
		return (1);
	}
	close(original_fd);
	return (0);
}

int	check_redirection(t_ASTnode *node)
{
	if (node->redirection != NULL)
		return (1);
	return (0);
}

/*PIPEX FONCTION FILE: utils.c*/
void	free_array(char **arr)
{
	size_t	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

char	**get_path(char **envp)
{
	while (*envp && ft_strnstr(*envp, "PATH", 5) == 0)
		envp++;
	if (!*envp)
	{
		printf("no find path\n");
		return (NULL);
	}
	return (ft_split(*envp + 5, ':'));
}

char	*find_path(char *cmd, char **envp)
{
	char	**paths;
	char	*path;
	char	*part_path;
	int		i;

	if (ft_strchr(cmd, '/'))
		return (cmd);
	paths = get_path(envp);
	if (!paths)
		return (NULL);
	i = -1;
	while (paths[++i])
	{
		part_path = clean_old_content(ft_strjoin(paths[i], "/"), false);
		path = clean_old_content(ft_strjoin(part_path, cmd), false);
		if (access(path, F_OK | X_OK) == 0)
		{
			free_array(paths);
			return (path);
		}
	}
	free_array(paths);
	return (NULL);
}
