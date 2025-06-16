/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   exec2.c											:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: yujin <yujin@student.42.fr>				+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2025/06/14 12:27:44 by yujin			 #+#	#+#			 */
/*   Updated: 2025/06/14 13:20:29 by yujin			###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "minishell.h"

void	safe_close(int *fd)
{
	if (*fd != -1)
	{
		close(*fd);
		*fd = -1;
	}
}

// Fonction auxiliaire : gère les redirections pour un builtin
static int	handle_builtin_redirection(t_shell *shell_program,
			t_ASTnode *cmd_node)
{
	int	redir_status;

	shell_program->stdin = dup(STDIN_FILENO);
	shell_program->stdout = dup(STDOUT_FILENO);
	if (!check_redirection(cmd_node))
		return (0);
	if (shell_program->stdin == -1 || shell_program->stdout == -1)
	{
		shell_program->exit_status = 1;
		perror("Dup failed in redirection");
		return (1);
	}
	redir_status = handle_redirection(shell_program, cmd_node);
	if (redir_status != 0)
	{
		close_and_restore(shell_program->stdout, STDOUT_FILENO);
		close_and_restore(shell_program->stdin, STDIN_FILENO);
		shell_program->exit_status = 1;
		safe_close(&shell_program->stdin);
		safe_close(&shell_program->stdout);
		return (1);
	}
	return (0);
}

// Fonction principale : exécute une commande builtin avec redirection
int	execute_builtin_in_shell(t_shell *shell_program, t_ASTnode *cmd_node)
{
	if (handle_builtin_redirection(shell_program, cmd_node) != 0)
		return (1);
	shell_program->exit_status = excute_builtin(shell_program,
			cmd_node->expanded_args);
	if (shell_program->stdout != -1)
	{
		close_and_restore(shell_program->stdout, STDOUT_FILENO);
		shell_program->stdout = -1;
	}
	if (shell_program->stdin != -1)
	{
		close_and_restore(shell_program->stdin, STDIN_FILENO);
		shell_program->stdin = -1;
	}
	return (shell_program->exit_status);
}
