#include "minishell.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "pipex.h"

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

void	error_commande(char *msg, int status)
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
		error_commande("command split failed", 1);
	path = find_path(cmd[0], envp);
	if (!path)
	{
		free_array(cmd);
		error_commande("command not found", 127);
	}
	if (execve(path, cmd, envp) == -1)
		error_commande("Execution failed", 126);
    free_array(cmd);
}

/*simple cmd ou un builtin*/
void dispatch_simple_command(t_shell *shell_program, ASTnode *ast)
{
    pid_t pid;
    int status;

    if (!ast || ast->type != CMD)
        shell_program->exit_status = 1;
    if (is_builtin(ast->args[0]))
        excute_builtin(shell_program, ast->args);
    pid = fork();
    if (pid == 0)
    {
        execute(ast->args[0], shell_program->environ);
        exit(0);
    }
    else if (pid > 0)
    {
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            shell_program->exit_status = WEXITSTATUS(status);
        else
            shell_program->exit_status = 1;
    }
    else
    {
        error_commande("fork error", 1);
        shell_program->exit_status = 1;
    }
    shell_program->exit_status = 0;
}


/*pipe*/
void dispatch_pipeline(t_shell *shell_program, ASTnode *ast)
{
    int fd[2];

    if (!ast || ast->type != PIPE)
        shell_program->exit_status = 1;
    if (pipe(fd) == -1)
        error_commande("pipe error", 1);
    if (fork() == 0)
    {
        dup2(fd[1], STDOUT_FILENO);
        close(fd[0]);
        dispatch_command(shell_program, ast->left);// 递归调度左子树
        exit(shell_program->exit_status);
    }
    if (fork() == 0)
    {
        dup2(fd[0], STDIN_FILENO);
        close(fd[1]);
        dispatch_command(shell_program, ast->right);// 递归调度右子树
        exit(shell_program->exit_status);
    }
    close(fd[0]);
    close(fd[1]);
    while (waitpid(-1, NULL, 0) > 0)
        ;
    shell_program->exit_status = 0;
}


/* séparer simple cmd:builtin et les restes exemple pipe*/
void dispatch_command(t_shell *shell_program, ASTnode *ast)
{
    if (!ast)
        shell_program->exit_status = 0;
    if (ast->type == CMD)
        dispatch_simple_command(shell_program, ast);
    else if (ast->type == PIPE)
        dispatch_pipeline(shell_program, ast);
    shell_program->exit_status = 0;
}
