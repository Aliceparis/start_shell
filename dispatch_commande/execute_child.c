/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_child.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yujin <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 11:59:56 by yujin             #+#    #+#             */
/*   Updated: 2025/06/14 11:59:58 by yujin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_parent_process_wait(t_shell *shell_program, pid_t pid)
{
	int	status;

	status = -1;
	setup_parent_for_child_execution();
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
	{
		shell_program->exit_status = WEXITSTATUS(status);
	}
	else if (WIFSIGNALED(status))
	{
		shell_program->exit_status = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGINT)
		{
			write(STDERR_FILENO, "\n", 1);
		}
		else if (WTERMSIG(status) == SIGQUIT)
		{
			write(STDERR_FILENO, "Quit (core dumped)\n", 19);
		}
	}
}

void	handle_child_process_execution(t_shell *shell_program,
			t_ASTnode *cmd_node, char *path)
{
	reset_signals_for_child();
	if (check_redirection(cmd_node) == 1)
	{
		if (handle_redirection(shell_program, cmd_node) != 0)
		{
			free_all(shell_program);
			exit(shell_program->exit_status);
		}
	}
	if (execve(path, cmd_node->expanded_args, shell_program->environ) == -1)
	{
		perror("minishell: execve failed");
		free(path);
		exit(127);
	}
}
