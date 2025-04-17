
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yujin <yujin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 12:41:49 by yujin             #+#    #+#             */
/*   Updated: 2025/03/28 12:17:25 by yujin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

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
}
