#include "minishell.h"
extern volatile sig_atomic_t g_signal_status;

static void	execute_heredoc_child(const char *delimiter,
		int write_fd, t_shell *shell_program)
{
	char	*line;

	reset_signals_for_child();
	while (true)
	{
		printf("heredoc child: entered readline loop\n");
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(write_fd, line, strlen(line));
		write(write_fd, "\n", 1);
		free(line);
	}
	close(write_fd);
	free_all(shell_program);
	exit(0);
}

void	heredoc_loop(const char *delimiter, int write_fd,
		t_shell *shell_program)
{
	char	*line;
	(void)shell_program;
	while (1)
	{
		
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(write_fd, line, strlen(line) + 1);
		write(write_fd, "\n", 1);
		free(line);
	}
	close(write_fd);
	exit(0);
}

static void	monitor_heredoc_child(pid_t pid, t_shell *shell_program,
		int read_fd)
{
	int	status;
	(void)read_fd;
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
	{
		shell_program->exit_status = WEXITSTATUS(status);
		if (shell_program->exit_status == 130)
			shell_program->heredoc_sigint = true;
	}	
	else if (WIFSIGNALED(status))
	{
		shell_program->exit_status = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGINT)
		{
			shell_program->heredoc_sigint = true;
			//close(read_fd);
			//write(STDERR_FILENO, "\n", 1);
		}
		else if (WTERMSIG(status) == SIGQUIT)
			write(STDERR_FILENO, "Quit (core dumped)\n", 19);
	}
}

int	start_heredoc(const char *delimiter, t_shell *shell_program)
{
	int		pipefd[2];
	pid_t	pid;

	shell_program->heredoc_sigint = false;
	if (pipe(pipefd) == -1)
		return (perror("pipe failed"), -1);
	set_heredoc_sig();
	pid = fork();
	if (pid == -1)
	{
		perror("fork failed");
		close(pipefd[0]);
		close(pipefd[1]);
		return (restore_parent_signals(), -1);
	}
	if (pid == 0)
	{
		close(pipefd[0]);
		//reset_signals_for_child();
		execute_heredoc_child(delimiter, pipefd[1], shell_program);
	}
	else
	{
		close(pipefd[1]);
		monitor_heredoc_child(pid, shell_program, pipefd[0]);
		restore_parent_signals();
	}
	if (shell_program->heredoc_sigint)
    {
        close(pipefd[0]);
        return (-1);
    }
	return (pipefd[0]);
}

/*void    parent_heredoc(pid_t pid, t_shell *shell_program, int read_fd)
{
    int status;

    status = -1;
    g_signal_status = SIGNAL_CHILD;
    waitpid(pid, &status, 0);
    g_signal_status = SIGNAL_NORMAL;
    if (WIFEXITED(status))
        shell_program->exit_status = WEXITSTATUS(status);
    else if (WIFSIGNALED(status))
        shell_program->exit_status = 128 + WTERMSIG(status);
    if (shell_program->exit_status == 130) // Ctrl+C
    close(read_fd);
}*/
int handle_heredoc_redirection(t_shell *shell_program, s_redict *redir)
{
    int heredoc_fd;

    heredoc_fd = start_heredoc(redir->delimiter, shell_program);
    if (heredoc_fd == -1)
	{
		shell_program->exit_status = 130;
        return (-1);
	}
    if (dup2(heredoc_fd, STDIN_FILENO) == -1)
    {
        close(heredoc_fd);
        error_message(shell_program, "dup2 for heredoc failed", 1);
        return (1);
    }
    close(heredoc_fd);
    return (0);
}