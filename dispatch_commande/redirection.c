/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   redirection.c									  :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: yujin <yujin@student.42.fr>				+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2025/06/07 20:02:15 by yujin			 #+#	#+#			 */
/*   Updated: 2025/06/14 11:42:59 by yujin			###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include"minishell.h"

/* Redirection >
 * Returns 0 on success, 1 on error */
int	handle_output_redirection(t_shell *shell_program, t_redict *redirection)
{
	int	fd;

	fd = open(redirection->delimiter, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		error_message(shell_program, "Redirection OUT: ouverture échouée", 1);
		return (1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		error_message(shell_program, "Erreur dup2 OUT", 1);
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

/* Redirection >>
 * Returns 0 on success, 1 on error*/
int	handle_append_redirection(t_shell *shell_program, t_redict *redirection)
{
	int	fd;

	fd = open(redirection->delimiter, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		error_message(shell_program, "Redirection APPEND: open faild", 1);
		return (1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		error_message(shell_program, "Erreur dup2 APPEND", 2);
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

/*Redirection <
 * Returns 0 on success, 1 on error*/
int	handle_input_redirection(t_shell *shell_program, t_redict *redirection)
{
	int	fd;

	fd = open(redirection->delimiter, O_RDONLY);
	(void)shell_program;
	if (fd < 0)
	{
		error_message(shell_program,
			"Redirection IN: No such file or directory", 1);
		return (1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		error_message(shell_program, "Erreur dup2 IN", 2);
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

/* Redirection heredoc
* return 0 sucess, 1 error*/

int	handle_heredoc_redirection(t_shell *shell_program, t_redict *redir)
{
	int	heredoc_fd;

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

/* Fonction principale de redirection */
int	handle_redirection(t_shell *shell_program, t_ASTnode *node)
{
	t_redict	*redir;
	int			result;

	redir = node->redirection;
	while (redir)
	{
		result = 1;
		if (redir->type == OUT)
			result = handle_output_redirection(shell_program, redir);
		else if (redir->type == APPEND)
			result = handle_append_redirection(shell_program, redir);
		else if (redir->type == IN)
			result = handle_input_redirection(shell_program, redir);
		else if (redir->type == HEREDOC)
			result = handle_heredoc_redirection(shell_program, redir);
		else
			return (error_message(shell_program,
					"Unknown redirection type", 1), 1);
		if (result != 0)
			return (1);
		redir = redir->next;
	}
	return (0);
}
