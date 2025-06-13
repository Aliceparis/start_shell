#include "minishell.h"
volatile sig_atomic_t g_heredoc_sigint = 0;

void	handle_heredoc_sigint_child(int sig)
{
	(void)sig;
	write(STDERR_FILENO, "^C\n", 3);
    g_heredoc_sigint = 1;
	close(0);
}
void   setup_heredoc_child(void)
{
    struct sigaction sa_heredoc;

    ft_bzero(&sa_heredoc, sizeof(sa_heredoc));
    sa_heredoc.sa_handler = handle_heredoc_sigint_child;
	sigemptyset(&sa_heredoc.sa_mask);
	sa_heredoc.sa_flags = SA_RESTART;
	if (sigaction(SIGINT, &sa_heredoc, NULL) == -1)
		perror("sigaction heredoc child SIGINT failed");
	signal(SIGQUIT, SIG_IGN);
}
static void	execute_heredoc_child(const char *delimiter,
		int write_fd, t_shell *shell_program)
{
	char	*line;
	int	i;
	i = 3;

	setup_heredoc_child();
	signal(SIGPIPE, SIG_IGN);
	while (i < 256)
	{
		if (i != write_fd)
			close(i);
		i++;
	}
	while (true)
	{
		line = readline("> ");
		if (!line || g_heredoc_sigint)
		{
			if (g_heredoc_sigint)
			{
				free_all(shell_program);
				shell_program->heredoc_sigint = true;
				shell_program->exit_status = 130;
				close(write_fd);
				exit(130);
			}
			break ;
		}
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

/*void	heredoc_loop(const char *delimiter, int write_fd,
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
}*/

static void	monitor_heredoc_child(pid_t pid, t_shell *shell_program,
		int read_fd)
{
	int	status = -1;
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
	setup_parent_for_child_execution();
	pid = fork();
	if (pid == -1)
	{
		perror("fork failed");
		close(pipefd[0]);
		close(pipefd[1]);
		setup_signe_parent();
		return ( -1);
	}
	if (pid == 0)
	{
		close(pipefd[0]);
		execute_heredoc_child(delimiter, pipefd[1], shell_program);
	}
	else
	{
		close(pipefd[1]);
		monitor_heredoc_child(pid, shell_program, pipefd[0]);
		setup_signe_parent();	
		if (shell_program->heredoc_sigint)
    	{
        	close(pipefd[0]);
			rl_replace_line("", 0);
			rl_on_new_line();
			shell_program->heredoc_sigint = false;
        	return (-1);
    	}
		shell_program->heredoc_sigint = false;
		return (pipefd[0]);
	}
	return (-1);
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

