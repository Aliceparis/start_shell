#include "minishell.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

void handle_redirection(t_shell *shell_program, ASTnode *ast);


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

static void	execute(char **cmd, char **envp, t_shell *shell_program)
{
	char	*path;

	if (!cmd)
		error_commande("command split failed", 1);
	path = find_path(cmd[0], envp);
	if (!path)
	{
		shell_program->exit_status = 1;
		error_commande("command not found", 127);
	}
	if (execve(path, cmd, envp) == -1)
    {
        shell_program->exit_status = 1;
		error_commande("Execution failed", 126);
    }
}

/*simple cmd ou un builtin*/
void dispatch_simple_command(t_shell *shell_program, ASTnode *ast)
{
    pid_t pid;
    int status;

    if (!ast || ast->type != CMD)
    {
        shell_program->exit_status = 1;
        return ;
    }
    if (is_builtin(ast->args[0]))
    {
        excute_builtin(shell_program, ast->args);
        return ;
    }
    pid = fork();
    if (pid == 0)
    {
        execute(ast->args, shell_program->environ, shell_program);
        shell_program->exit_status = 0;
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
		error_message(shell_program, "fork error", 1);
        shell_program->exit_status = 1;
    }
}


/*pipe*/
void dispatch_pipeline(t_shell *shell_program, ASTnode *ast)
{
    int fd[2];
    pid_t pid1, pid2;

    if (!ast || ast->type != PIPE){
        shell_program->exit_status = 1;
        return ;
    }
    if (pipe(fd) == -1)
        error_message(shell_program, "pipe error", 1);
    pid1 = fork();
    if (pid1 == 0)
    {
        dup2(fd[1], STDOUT_FILENO);
        close(fd[0]);
		close(fd[1]);
        dispatch_command(shell_program, ast->left);// 递归调度左子树
        exit(shell_program->exit_status);
    }
    pid2 = fork();
    if (pid2 == 0)
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
    {
        shell_program->exit_status = 0;
        return ;
    }
    if (ast->type == CMD)
        dispatch_simple_command(shell_program, ast);
    else if (ast->type == PIPE)
        dispatch_pipeline(shell_program, ast);
    else if(ast->r_type == HEREDOC)
    {
        start_heredoc(ast->delimiter);
        if (g_heredoc_interrupted != 1)
            dispatch_command(shell_program, ast->left);
    }
    else if (ast->type == REDIRECTION)
        handle_redirection(shell_program,ast); 
    shell_program->exit_status = 0;
}


void handle_redirection(t_shell *shell_program, ASTnode *ast)
{
	int fd;

	if (!ast || ast->type != REDIRECTION)
		return;

	if (ast->r_type == OUT)
	{
		fd = open(ast->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd < 0)
			return (error_message(shell_program, "Redirection OUT: ouverture échouée\n", 1));
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	else if (ast->r_type == APPEND)
	{
		fd = open(ast->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd < 0)
			return (error_message(shell_program, "Redirection APPEND: ouverture échouée\n", 1));
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	else if (ast->r_type == IN)
	{
		fd = open(ast->file, O_RDONLY);
		if (fd < 0)
			return (error_message(shell_program, "Redirection IN: fichier introuvable\n", 1));
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	dispatch_command(shell_program, ast->left);
}
