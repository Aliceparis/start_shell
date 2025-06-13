#include "minishell.h"
static int create_pipe(t_shell *shell_program, int *fd)
{
    if (pipe(fd) == -1)
    {
        perror("Create pipe failed");
        return (shell_program->exit_status = 1, 1);
    }
    return (0);
}

static void    wait_and_return_status(t_shell *shell_program, pid_t pid1, pid_t pid2)
{
    int status1;
    int status2;

    status1 = -1;
    status2 = -1;
    waitpid(pid1, &status1, 0);
    waitpid(pid2, &status2, 0);
    if (WIFEXITED(status2))
    {
        shell_program->exit_status = WEXITSTATUS(status2);
    }
    else if (WIFSIGNALED(status2))
    {
        shell_program->exit_status = 128 + WTERMSIG(status2);
        if (WTERMSIG(status2) == SIGINT)
        {
            if (g_heredoc_sigint)
            {
                free_all(shell_program);
                g_heredoc_sigint = 0;
            }
            // If the last command was terminated by SIGINT (Ctrl+C)
            write(STDERR_FILENO, "\n", 1); // Print newline for clean prompt on terminal
        }
        else if (WTERMSIG(status2) == SIGQUIT)
        {
            // If the last command was terminated by SIGQUIT (Ctrl+\)
            write(STDERR_FILENO, "Quit (core dumped)\n", 19);
        }
    }
}
static pid_t	fork_and_execute_left(t_shell *shell_program,
			ASTnode *left, int *fd)
{
    pid_t   pid;

    pid = fork();
    if (pid < 0)
    {
        perror("fork failed for left pipe child");
        return (shell_program->exit_status = 1, -1);
    }
    if (pid ==0)
    {
        reset_signals_for_child();
        close(fd[0]);
        if (check_redirection(left))
        {
            if (handle_redirection(shell_program, left) != 0)
            {
                printf("je rentre dans handle\n");
                free_all(shell_program);
                close(fd[1]);
                exit(1);
            }
        }
		if (dup2(fd[1], STDOUT_FILENO) == -1)
		{
			perror("dup2 pipe write end");
			free_all(shell_program);
            close(fd[1]);
			exit(1);
		}
        close(fd[1]);
        ft_execute_node(shell_program, left);
        free_all(shell_program);
        exit(shell_program->exit_status);
    }
    return (pid);
}

static pid_t	fork_and_execute_right(t_shell *shell_program,
		ASTnode *right, int *fd)
{
	pid_t	pid;

	pid = fork();

	if (pid < 0)
	{
		error_message(shell_program, "fork error", 1);
		return (-1);
	}
	if (pid == 0)
	{
		reset_signals_for_child();
        close(fd[1]);
        if (check_redirection(right))
        {
            if (handle_redirection(shell_program, right) != 0)
            {
                printf("je rentre dans handle\n");
                free_all(shell_program);
                close(fd[0]);
                exit(1);
            }
        }
		if (dup2(fd[0], STDIN_FILENO) == -1)
        {
            perror("DUP2 pipe failed in right");
            free_all(shell_program);
            close(fd[0]);
            exit(1);
        }
		close(fd[0]);
		ft_execute_node(shell_program, right);
		free_all(shell_program);
		exit(shell_program->exit_status);
	}
	return (pid);
}
int execute_pipeline(t_shell *shell_program, ASTnode *node)
{
    int fd[2];
    pid_t pid1;
    pid_t pid2;

    signal(SIGPIPE, SIG_IGN);
    if (!node || node->type != PIPE)
        return (shell_program->exit_status = 1);
    if (create_pipe(shell_program, fd) != 0)
        return (shell_program->exit_status);
    setup_parent_for_child_execution();
    pid1 = fork_and_execute_left(shell_program, node->left, fd);
    if (pid1 < 0)
    {
        close(fd[0]);
        close(fd[1]);
        return(shell_program->exit_status = 1);
    }
    pid2 = fork_and_execute_right(shell_program, node->right, fd);
    if (pid2 < 0)
    {
        close(fd[0]);
        close(fd[1]);
        waitpid(pid1, NULL, 0);
        return (shell_program->exit_status = 1);
    }
    close(fd[0]);
    close(fd[1]);
    wait_and_return_status(shell_program, pid1, pid2);
    setup_signe_parent();
    signal(SIGPIPE, SIG_DFL);
    return (shell_program->exit_status);
}
