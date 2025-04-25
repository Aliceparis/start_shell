#include "pipex.h"

void	child_process(char **argv, char **envp, int *fd)
{
	int		filein;

	filein = open(argv[1], O_RDONLY, 0777);
	if (filein == -1)
		error("Error opening input file", 127);
	dup2(fd[1], STDOUT_FILENO);
	dup2(filein, STDIN_FILENO);
	close(fd[0]);
	execute(argv[2], envp);
}

void	parent_process(char **argv, char **envp, int *fd)
{
	int		fileout;

	fileout = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (fileout == -1)
		error("Error opening output file", 127);
	dup2(fd[0], STDIN_FILENO);
	dup2(fileout, STDOUT_FILENO);
	close(fd[1]);
	execute(argv[3], envp);
}

void	fork_process(void (process)(char **, char **, int *),
								char **argv, char **envp, int *fd)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		error("Fork error", 127);
	if (pid == 0)
	{
		process(argv, envp, fd);
		exit(0);
	}
}

void	pipex(char **argv, char **envp)
{
	int		fd[2];

	if (pipe(fd) == -1)
		error("Pipe error", 127);
	fork_process(child_process, argv, envp, fd);
	fork_process(parent_process, argv, envp, fd);
	close(fd[0]);
	close(fd[1]);
	while (waitpid(-1, NULL, 0) > 0)
		;
}
/*
int	main(int argc, char **argv, char **envp)
{
	if (argc != 5)
	{
		ft_putstr_fd("Error: Bad arguments\n", 2);
		ft_putstr_fd("Ex: ./pipex <file1> <cmd1> <cmd2> <file2>\n", 2);
		return (1);
	}
	pipex(argv, envp);
	return (0);
}*/
/*pipex_util.c*/
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