/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yujin <yujin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 12:27:31 by yujin             #+#    #+#             */
/*   Updated: 2025/06/14 15:07:43 by yujin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_execute_node(t_shell *shell_program, t_ASTnode *node)
{
	if (!node)
		return ;
	if (node->type == PIPE)
		execute_pipeline(shell_program, node);
	else
		execute_simple_command(shell_program, node);
}

int	execute_simple_command(t_shell *shell_program, t_ASTnode *node)
{
	if (node->expanded_args == NULL)
		return (execute_redirection(shell_program, node));
	if (is_builtin(node->expanded_args[0]))
		return (execute_builtin_in_shell(shell_program, node));
	else
	{
		execute_externe_command(shell_program, node);
		return (shell_program->exit_status);
	}
}

int	execute_redirection(t_shell *shell_program, t_ASTnode *node)
{
	int	redir_status;

	if (check_redirection(node))
	{
		if (shell_program->stdin == -1 || shell_program->stdout == -1)
		{
			shell_program->exit_status = 1;
			return (perror("Dup failed in redirection"), 1);
		}
		redir_status = handle_redirection(shell_program, node);
		if (shell_program->stdout != -1)
			close_and_restore(shell_program->stdout, STDOUT_FILENO);
		if (shell_program->stdin != -1)
			close_and_restore(shell_program->stdin, STDIN_FILENO);
		if (redir_status != 0)
			return (shell_program->exit_status = 1);
	}
	shell_program->exit_status = 0;
	return (0);
}

static pid_t	fork_and_execute_child(t_shell *shell_program,
				t_ASTnode *cmd_node, char **path_out)
{
	pid_t	pid;

	if (!cmd_node)
	{
		error_message(shell_program, "command split failed", 1);
		return (-1);
	}
	*path_out = find_path(cmd_node->expanded_args[0], shell_program->environ);
	if (!*path_out)
	{
		error_message(shell_program, "command not found", 127);
		return (-1);
	}
	setup_parent_for_child_execution();
	pid = fork();
	if (pid == -1)
	{
		error_message(shell_program, "fork failed", 1);
		free(*path_out);
		setup_signe_parent();
		return (-1);
	}
	if (pid == 0)
		handle_child_process_execution(shell_program, cmd_node, *path_out);
	return (pid);
}

void	execute_externe_command(t_shell *shell_program, t_ASTnode *cmd_node)
{
	char	*path;
	pid_t	pid;

	path = NULL;
	pid = fork_and_execute_child(shell_program, cmd_node, &path);
	if (pid == -1)
		return ;
	if (pid > 0)
	{
		handle_parent_process_wait(shell_program, pid);
		setup_signe_parent();
	}
}
