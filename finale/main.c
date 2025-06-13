#include "minishell.h"

void	init_shell(t_shell *shell_program, char **env)
{
	shell_program->environ = env;
	shell_program->envlist = NULL;
	shell_program->token_list = NULL;
	shell_program->current_token = NULL;
	shell_program->ast = NULL;
	shell_program->exit_status = 0;
	shell_program->heredoc_sigint = false;
	init_envlist(shell_program);
	tcgetattr(STDIN_FILENO, &shell_program->oldt);
	//tcsetattr(STDIN_FILENO, TCSANOW, &shell_program->oldt);
	struct termios newt = shell_program->oldt;
	newt.c_lflag |=  ECHOCTL; 
	newt.c_lflag &= ~ICANON;// Désactiver l'écho des caractères de contrôle
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	shell_program->stdin = dup(STDIN_FILENO);
	shell_program->stdout = dup(STDOUT_FILENO);

}


void    reset_terminal(t_shell *shell_program)
{
	tcsetattr(STDIN_FILENO, TCSANOW, &shell_program->oldt);
}


int main(int ac, char **av, char **envp)
{
	t_shell	shell_program;

	rl_catch_signals = 0;
	((void)ac, (void)av);
	init_shell(&shell_program, envp);
	setup_signe_parent();
	//ft_init_signals(&shell_program);
	while (1)
	{
		shell_program.line = readline("Minishell> ");

		if (!shell_program.line)
		{
			write(STDOUT_FILENO, "exit\n", 5); // 打印 "exit" 以获得干净的退出外观
			break ; // 退出主 shell 循环
		}
		// 如果行是空的（用户只是按了 Enter），则释放它并再次提示
		if (shell_program.line[0] == '\0')
		{
			free(shell_program.line);
			shell_program.line = NULL;
			continue; 
		}
		add_history(shell_program.line);
		ft_token(shell_program.line, &shell_program);
		if (shell_program.token_list)
		{
			shell_program.ast = ft_parse(&shell_program);
			if (shell_program.ast)
				start_execution(&shell_program);
		}
		free_ast_token(&shell_program);
		free(shell_program.line);
		shell_program.line = NULL;
		//free_all(&shell_program);
	}
	free_all(&shell_program);
	// 恢复终端设置
	rl_clear_history();
	reset_terminal(&shell_program);
	return 0;
}
