/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yujin <yujin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 12:20:51 by yujin             #+#    #+#             */
/*   Updated: 2025/06/14 17:44:01 by yujin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Sous-fonction : gère l'interruption par SIGINT
void	handle_heredoc_interrupt(t_shell *shell_program, int write_fd)
{
	free_all(shell_program);
	shell_program->heredoc_sigint = true;
	shell_program->exit_status = 130;
	close(write_fd);
	exit(130);
}

// Fonction séparée : boucle de lecture du here-document
void	heredoc_read_loop(const char *delimiter,
			int write_fd, t_shell *shell_program)
{
	char	*line;

	while (true)
	{
		line = readline("> ");
		if (!line || g_heredoc_sigint)
		{
			if (g_heredoc_sigint)
				handle_heredoc_interrupt(shell_program, write_fd);
			free(line);
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
}

// Fonction principale : processus enfant du heredoc
void	execute_heredoc_child(const char *delimiter,
			int write_fd, t_shell *shell_program)
{
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
	heredoc_read_loop(delimiter, write_fd, shell_program);
	close(write_fd);
	free_all(shell_program);
	signal(SIGPIPE, SIG_DFL);
	exit(0);
}

void	monitor_heredoc_child(pid_t pid, t_shell *shell_program,
		int read_fd)
{
	int	status;

	status = -1;
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
