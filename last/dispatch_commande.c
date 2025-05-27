#include "minishell.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

void handle_redirection(t_shell *shell_program, ASTnode *ast);
extern  volatile sig_atomic_t g_signal;

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
	if (!*envp){
        printf("je ne trouve pas le path\n");
		return (NULL);}
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
		free_all(shell_program);
		error_commande("command not found", 127);
        shell_program->exit_status = 1;
	}
	if (execve(path, cmd, envp) == -1)
    {
        free_all(shell_program);
        shell_program->exit_status = 1;
		error_commande("Execution failed", 126);
    }
}

/*simple cmd ou un builtin*/
void dispatch_simple_command(t_shell *shell_program, ASTnode *ast)
{
    pid_t pid;
    int status = 0;

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
    g_signal = PIPE;
    pid = fork();
    if (pid == 0)
    {
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        execute(ast->args, shell_program->environ, shell_program);
        free_all(shell_program);
    }
    else if (pid > 0)
    {
        waitpid(pid, &status, 0);
        /*while (waitpid(pid, &status, 0) == -1)
        {
            if (errno == EINTR)
                continue;
            break;
        }*/
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

void close_pipe_fds(int fd[2])
{
    if (fd[0] != -1)
        close(fd[0]);
    if (fd[1] != -1)
        close(fd[1]);
}

static pid_t fork_and_execute_left(t_shell *shell_program, ASTnode *left, int *fd)
{
    pid_t pid;

   
    pid = fork();
    if (pid < 0)
    {
        error_message(shell_program, "fork error", 1);
        return (-1);
    }
    if (pid == 0)
    {
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        dup2(fd[1], STDOUT_FILENO);
        close_pipe_fds(fd);
        dispatch_command(shell_program, left);
        free_all(shell_program);
        exit(shell_program->exit_status);
    }
    return (pid);
}

static pid_t fork_and_execute_right(t_shell *shell_program, ASTnode *right, int *fd)
{
    pid_t pid;

    pid = fork();
    if (pid < 0)
    {
        error_message(shell_program, "fork error", 1);
        return (-1);
    }
    if (pid == 0)
    {
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        dup2(fd[0], STDIN_FILENO);
        close_pipe_fds(fd);
        dispatch_command(shell_program, right);
        free_all(shell_program);
        exit(shell_program->exit_status);
    }
    return (pid);
}

void dispatch_pipeline(t_shell *shell_program, ASTnode *ast)
{
    int fd[2];
    pid_t pid1;
    pid_t pid2;
    int status1 = 0;
    int status2 = 0;

    g_signal = PIPE;
    if (!ast || ast->type != PIPE)
    {
        shell_program->exit_status = 1;
        return ;
    }
    if (pipe(fd) == -1) 
    {
        error_message(shell_program, "pipe error", 1);
        shell_program->exit_status = 1;
        return ;
    }
    pid1 = fork_and_execute_left(shell_program, ast->left, fd);
    if (pid1 < 0)
    {
        close_pipe_fds(fd);
        shell_program->exit_status = 1;
        g_signal = NORMAL;
        return ;
    }
    pid2 = fork_and_execute_right(shell_program, ast->right, fd);
    if (pid2 < 0)
    {
        close_pipe_fds(fd);
        shell_program->exit_status = 1;
        g_signal = NORMAL;

        return ;
    }
    close_pipe_fds(fd);
    /*while (waitpid(pid2, &status2, 0) == -1)
    {
        if (errno == EINTR)
            continue;
        break;
    }
    while (waitpid(pid1, &status1, 0) == -1)
    {
        if (errno == EINTR)
            continue;
        break;
    }*/
    waitpid(pid2, &status2, 0);
    waitpid(pid1, &status1, 0);
    if (WIFEXITED(status2))
        shell_program->exit_status = WEXITSTATUS(status2);
    else
        shell_program->exit_status = 1;
    g_signal = 0;
}

/* séparer simple cmd:builtin et les restes exemple pipe*/
void dispatch_command(t_shell *shell_program, ASTnode *ast)
{
    if (!ast)
        return ;
    if (ast->type == CMD)
    {
        dispatch_simple_command(shell_program, ast);
        //free_all(shell_program);
    }
    else if (ast->type == PIPE)
        dispatch_pipeline(shell_program, ast);
    else if(ast->type == HEREDOC)
    {
        start_heredoc(ast->delimiter, shell_program);
        dispatch_command(shell_program, ast->left);
    }
    else
        handle_redirection(shell_program,ast); 
}

void close_and_restore(int original_fd, int std_fd)
{
    if (dup2(original_fd, std_fd) == -1)
        perror("Erreur lors de la restauration du FD");
}

// Redirection >
void handle_output_redirection(t_shell *shell_program, ASTnode *ast)
{
    int fd = open(ast->delimiter, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    int saved_stdout = dup(STDOUT_FILENO);

    if (saved_stdout == -1 || fd < 0)
    {
        error_message(shell_program, "Redirection OUT: ouverture échouée\n", 1);
        if (saved_stdout != -1) close(saved_stdout);
        if (fd != -1) close(fd);
        return;
    }
    if (dup2(fd, STDOUT_FILENO) == -1)
    {
        perror("Erreur dup2 OUT");
        close(fd);
        close(saved_stdout);
        return;
    }
    close(fd);
     int old_signal = g_signal;
    g_signal = PIPE;
    dispatch_command(shell_program, ast->left);
    g_signal = old_signal;

    close_and_restore(saved_stdout, STDOUT_FILENO);
}

// Redirection >>
void handle_append_redirection(t_shell *shell_program, ASTnode *ast)
{
    int fd = open(ast->delimiter, O_WRONLY | O_CREAT | O_APPEND, 0644);
    int saved_stdout = dup(STDOUT_FILENO);

    if (saved_stdout == -1 || fd < 0)
    {
        error_message(shell_program, "Redirection APPEND: ouverture échouée\n", 1);
        if (saved_stdout != -1) close(saved_stdout);
        if (fd != -1) close(fd);
        return;
    }

    if (dup2(fd, STDOUT_FILENO) == -1)
    {
        perror("Erreur dup2 APPEND");
        close(fd);
        close(saved_stdout);
        return;
    }
    close(fd);
    dispatch_command(shell_program, ast->left);
    close_and_restore(saved_stdout, STDOUT_FILENO);
}

// Redirection <
void handle_input_redirection(t_shell *shell_program, ASTnode *ast)
{
    int fd = open(ast->delimiter, O_RDONLY);
    int saved_stdin = dup(STDIN_FILENO);

    if (saved_stdin == -1 || fd < 0)
    {
        error_message(shell_program, "Redirection IN: fichier introuvable\n", 1);
        if (saved_stdin != -1) close(saved_stdin);
        if (fd != -1) close(fd);
        return;
    }

    if (dup2(fd, STDIN_FILENO) == -1)
    {
        perror("Erreur dup2 IN");
        close(fd);
        close(saved_stdin);
        return;
    }
    close(fd);
    dispatch_command(shell_program, ast->left);
    close_and_restore(saved_stdin, STDIN_FILENO);
}

// Fonction principale de redirection
void handle_redirection(t_shell *shell_program, ASTnode *ast)
{
    if (ast->type == OUT)
        handle_output_redirection(shell_program, ast);
    else if (ast->type == APPEND)
        handle_append_redirection(shell_program, ast);
    else if (ast->type == IN)
        handle_input_redirection(shell_program, ast);
}
