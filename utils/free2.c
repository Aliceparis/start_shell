/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yujin <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 13:33:26 by yujin             #+#    #+#             */
/*   Updated: 2025/06/14 13:33:28 by yujin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_envlist(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env;
		env = env->next;
		free(tmp);
	}
	env = NULL;
}

void	free_all(t_shell *shell_program)
{
	clean_old_content(NULL, true);
	free_ast_token(shell_program);
	if (shell_program->envlist)
	{
		free_envlist(shell_program->envlist);
		shell_program->envlist = NULL;
	}
	if (shell_program->line)
	{
		free(shell_program->line);
		shell_program->line = NULL;
	}
	if (shell_program->stdin)
		safe_close(&shell_program->stdin);
	if (shell_program->stdout)
		safe_close(&shell_program->stdout);
	tcsetattr(STDIN_FILENO, TCSANOW, &shell_program->oldt);
}
