/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yujin <yujin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 13:46:10 by yujin             #+#    #+#             */
/*   Updated: 2025/06/14 18:09:43 by yujin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_shell(t_shell *shell_program, char **env)
{
	struct termios	newt;

	shell_program->environ = env;
	shell_program->envlist = NULL;
	shell_program->token_list = NULL;
	shell_program->current_token = NULL;
	shell_program->ast = NULL;
	shell_program->exit_status = 0;
	shell_program->heredoc_sigint = false;
	init_envlist(shell_program);
	tcgetattr(STDIN_FILENO, &shell_program->oldt);
	newt = shell_program->oldt;
	newt.c_lflag |= ECHOCTL;
	newt.c_lflag &= ~ICANON;
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	shell_program->stdin = dup(STDIN_FILENO);
	shell_program->stdout = dup(STDOUT_FILENO);
}

void	reset_terminal(t_shell *shell_program)
{
	tcsetattr(STDIN_FILENO, TCSANOW, &shell_program->oldt);
}

void	process_shell_line(t_shell *shell_program)
{
	if (shell_program->token_list)
	{
		shell_program->ast = ft_parse(shell_program);
		if (shell_program->ast)
			start_execution(shell_program);
	}
	free_ast_token(shell_program);
	free(shell_program->line);
	shell_program->line = NULL;
}

void	shell_loop(t_shell *shell_program)
{
	while (1)
	{
		shell_program->line = readline("Minishell> ");
		if (g_heredoc_sigint == 1)
		{
			shell_program->exit_status = 130;
			g_heredoc_sigint = 0;
			free(shell_program->line);
			continue ;
		}
		if (!shell_program->line)
		{
			write(STDOUT_FILENO, "exit\n", 5);
			break ;
		}
		if (shell_program->line[0] == '\0')
		{
			free(shell_program->line);
			shell_program->line = NULL;
			continue ;
		}
		add_history(shell_program->line);
		ft_token(shell_program->line, shell_program);
		process_shell_line(shell_program);
	}
}

int	main(int ac, char **av, char **envp)
{
	t_shell	shell_program;

	rl_catch_signals = 0;
	(void)ac;
	(void)av;
	init_shell(&shell_program, envp);
	setup_signe_parent();
	shell_loop(&shell_program);
	free_all(&shell_program);
	rl_clear_history();
	reset_terminal(&shell_program);
	return (0);
}
