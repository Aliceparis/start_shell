/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dispatch_command.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loulou <loulou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 16:51:19 by loulou            #+#    #+#             */
/*   Updated: 2025/04/22 16:00:31 by loulou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


#include "../includes/pipex.h"
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
    free_array(cmd);
}

/*simple cmd ou un builtin*/
int dispatch_simple_command(t_shell *shell, t_node *ast)
{
    pid_t pid;
    int status;

    if (!ast || ast->type != CMD)
        return (1);
    if (is_builtin(ast->args[0]))
        return (excute_builtin(shell, ast->args));
    pid = fork();
    if (pid == 0)
    {
        execute(ast->args[0], shell->environ);
        exit(0);
    }
    else if (pid > 0)
    {
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            shell->exit_status = WEXITSTATUS(status);
        else
            shell->exit_status = 1;
    }
    else
    {
        error("fork error", 1);
        shell->exit_status = 1;
    }
    return (0);
}


/*pipe*/
int dispatch_pipeline(t_shell *shell, t_node *ast)
{
    int fd[2];

    if (!ast || ast->type != PIPE)
        return (1);
    if (pipe(fd) == -1)
        error("pipe error", 1);
    if (fork() == 0)
    {
        dup2(fd[1], STDOUT_FILENO);
        close(fd[0]);
        dispatch_command(shell, ast->left);// 递归调度左子树
        exit(shell->exit_status);
    }
    if (fork() == 0)
    {
        dup2(fd[0], STDIN_FILENO);
        close(fd[1]);
        dispatch_command(shell, ast->right);// 递归调度右子树
        exit(shell->exit_status);
    }
    close(fd[0]);
    close(fd[1]);
    while (waitpid(-1, NULL, 0) > 0)
        ;
    return (0);
}


/* séparer simple cmd:builtin et les restes exemple pipe*/
int dispatch_command(t_shell *shell, t_node *ast)
{
    if (!ast)
        return (0);
    if (ast->type == CMD)
        return dispatch_simple_command(shell, ast);
    else if (ast->type == PIPE)
        return dispatch_pipeline(shell, ast);
    return (0);
}
