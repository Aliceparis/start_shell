/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yujin <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 12:20:45 by yujin             #+#    #+#             */
/*   Updated: 2025/06/14 12:20:46 by yujin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Fonction auxiliaire : crée pipe + fork, lance le processus fils
static pid_t	fork_heredoc_process(const char *delimiter,
				t_shell *shell_program, int *pipefd)
{
	pid_t	pid;

	if (pipe(pipefd) == -1)
	{
		error_message(shell_program, "pipe failed", 1);
		return (-1);
	}
	setup_parent_for_child_execution();
	pid = fork();
	if (pid == -1)
	{
		perror("fork failed");
		close(pipefd[0]);
		close(pipefd[1]);
		setup_signe_parent();
		return (-1);
	}
	if (pid == 0)
	{
		close(pipefd[0]);
		execute_heredoc_child(delimiter, pipefd[1], shell_program);
	}
	return (pid);
}

int	start_heredoc(const char *delimiter, t_shell *shell_program)
{
	int		pipefd[2];
	pid_t	pid;

	shell_program->heredoc_sigint = false;
	pid = fork_heredoc_process(delimiter, shell_program, pipefd);
	if (pid < 0)
		return (-1);
	if (pid > 0)
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
