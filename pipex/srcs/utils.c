/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yujin <yujin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 12:32:35 by yujin             #+#    #+#             */
/*   Updated: 2025/03/28 15:17:30 by yujin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	free_array(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

char	*find_path(char *cmd, char **envp)
{
	char	**paths;
	char	*path;
	char	*part_path;
	int		i;

	if (ft_strchr(cmd, '/'))
		return (cmd);
	while (*envp && ft_strnstr(*envp, "PATH", 5) == 0)
		envp++;
	if (!*envp)
		return (NULL);
	paths = ft_split(*envp + 5, ':');
	if (!paths)
		return (NULL);
	i = -1;
	while (paths[++i])
	{
		part_path = ft_strjoin(paths[i], "/");
		path = ft_strjoin(part_path, cmd);
		free(part_path);
		if (!access(path, F_OK | X_OK))
			return (free_array(paths), path);
		free(path);
	}
	return (free_array(paths), NULL);
}

void	error(char *msg, int status)
{
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
	exit(status);
}

void	execute(char *argv, char **envp)
{
	char	**cmd;
	char	*path;

	cmd = ft_split(argv, ' ');
	if (!cmd)
		error("command split failed", 1);
	path = find_path(cmd[0], envp);
	if (!path)
	{
		free_array(cmd);
		error("command not found", 127);
	}
	if (execve(path, cmd, envp) == -1)
		error("Execution failed", 126);
}
