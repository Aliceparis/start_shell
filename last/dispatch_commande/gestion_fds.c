#include "minishell.h"

void	close_and_restore(int original_fd, int std_fd)
{
	if (dup2(original_fd, std_fd) == -1)
		perror("Erreur lors de la restauration du FD");
}

void	close_pipe_fds(int fd[2])
{
	if (fd[0] != -1)
		close(fd[0]);
	if (fd[1] != -1)
		close(fd[1]);
}

int	check_redirection(ASTnode *node)
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
		return (printf("no find path\n"), NULL);
	paths = ft_split(*envp + 5, ':');
	if (!paths)
		return (NULL);
	i = -1;
	while (paths[++i])
	{
		part_path = clean_old_content(ft_strjoin(paths[i], "/"), false);
		path = clean_old_content(ft_strjoin(part_path, cmd), false);
		if (access(path, F_OK | X_OK) == 0)
			return (path);
		//free_array(paths);
	}
	return (NULL);
}
