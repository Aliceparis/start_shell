/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yujin <yujin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 12:07:42 by yujin             #+#    #+#             */
/*   Updated: 2025/06/14 15:06:20 by yujin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static pid_t	fork_and_execute_left(t_shell *shell_program,
			t_ASTnode *left, int *fd)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		error_message(shell_program, "fork failed for left pipe child", 1);
		return (-1);
	}
	if (pid == 0)
		child_execute_pipe_left(shell_program, left, fd);
	return (pid);
}

// Fonction principale pour la branche droite
static pid_t	fork_and_execute_right(t_shell *shell_program,
			t_ASTnode *right, int *fd)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		error_message(shell_program, "fork error", 1);
		return (-1);
	}
	if (pid == 0)
		child_execute_pipe_right(shell_program, right, fd);
	return (pid);
}

// Fonction auxiliaire pour exécuter les deux processus dans le pipeline
static int	execute_pipeline_processes(t_shell *shell_program,
			t_ASTnode *node, int *fd)
{
	pid_t	pid1;
	pid_t	pid2;

	pid1 = fork_and_execute_left(shell_program, node->left, fd);
	if (pid1 < 0)
	{
		close(fd[0]);
		close(fd[1]);
		return (1);
	}
	pid2 = fork_and_execute_right(shell_program, node->right, fd);
	if (pid2 < 0)
	{
		close(fd[0]);
		close(fd[1]);
		waitpid(pid1, NULL, 0);
		return (1);
	}
	close(fd[0]);
	close(fd[1]);
	wait_and_return_status(shell_program, pid1, pid2);
	return (0);
}

// Fonction principale qui gère l’exécution d’un pipeline
int	execute_pipeline(t_shell *shell_program, t_ASTnode *node)
{
	int	fd[2];

	signal(SIGPIPE, SIG_IGN);
	if (!node || node->type != PIPE)
		return (shell_program->exit_status = 1);
	if (create_pipe(shell_program, fd) != 0)
		return (shell_program->exit_status);
	setup_parent_for_child_execution();
	if (execute_pipeline_processes(shell_program, node, fd) != 0)
	{
		setup_signe_parent();
		signal(SIGPIPE, SIG_DFL);
		return (shell_program->exit_status = 1);
	}
	setup_signe_parent();
	signal(SIGPIPE, SIG_DFL);
	return (shell_program->exit_status);
}
