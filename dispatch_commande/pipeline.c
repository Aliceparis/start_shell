/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yujin <yujin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 12:07:50 by yujin             #+#    #+#             */
/*   Updated: 2025/06/14 15:06:45 by yujin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	create_pipe(t_shell *shell_program, int *fd)
{
	if (pipe(fd) == -1)
	{
		perror("Create pipe failed");
		return (shell_program->exit_status = 1, 1);
	}
	return (0);
}

void	wait_and_return_status(t_shell *shell_program,
			pid_t pid1, pid_t pid2)
{
	int	status1;
	int	status2;

	status1 = -1;
	status2 = -1;
	waitpid(pid1, &status1, 0);
	waitpid(pid2, &status2, 0);
	if (WIFEXITED(status2))
		shell_program->exit_status = WEXITSTATUS(status2);
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
			write(STDERR_FILENO, "\n", 1);
		}
		else if (WTERMSIG(status2) == SIGQUIT)
			write(STDERR_FILENO, "Quit (core dumped)\n", 19);
	}
}

// Fonction enfant pour exécuter la branche gauche du pipe
void	child_execute_pipe_left(t_shell *shell_program,
			t_ASTnode *left, int *fd)
{
	reset_signals_for_child();
	close(fd[0]);
	if (check_redirection(left) && handle_redirection(shell_program, left) != 0)
	{
		free_all(shell_program);
		close(fd[1]);
		exit(1);
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

// Fonction enfant pour exécuter la branche droite du pipe
void	child_execute_pipe_right(t_shell *shell_program,
			t_ASTnode *right, int *fd)
{
	reset_signals_for_child();
	close(fd[1]);
	if (check_redirection(right)
		&& handle_redirection(shell_program, right) != 0)
	{
		free_all(shell_program);
		close(fd[0]);
		exit(1);
	}
	if (dup2(fd[0], STDIN_FILENO) == -1)
	{
		perror("dup2 pipe read end");
		free_all(shell_program);
		close(fd[0]);
		exit(1);
	}
	close(fd[0]);
	ft_execute_node(shell_program, right);
	free_all(shell_program);
	exit(shell_program->exit_status);
}
