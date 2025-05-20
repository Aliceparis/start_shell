#include "minishell.h"
t_shell shell_program;

static struct termios oldt;

static void	ft_sigint_handler(int num)
{
	(void)num;
	if (shell_program.signint_child)
	{
		//ft_putstr_fd("\n", 1);
		shell_program.signint_child = false;
		shell_program.heredoc_sigint = true;
	}
	else
	{
		ft_putstr_fd("\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		close(0);
	}
}
void    init_shell(t_shell *shell_program, char **env)
{
    shell_program->environ = env;
	shell_program->envlist = NULL;
	shell_program->token_list = NULL;
	shell_program->ast = NULL;
	init_envlist(shell_program);
	shell_program->stdin = dup(STDIN_FILENO);
    shell_program->stdout = dup(STDOUT_FILENO);
    tcgetattr(STDIN_FILENO, &shell_program->oldt);
}

void reset_terminal(void)
{
	rl_catch_signals = 0;
    rl_catch_sigwinch = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // 恢复原始设置
}
void	ft_init_signals(t_shell *shell_program)
{
	struct termios	term;

	term = shell_program->oldt;
	term.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
	shell_program->heredoc_sigint = false;
	shell_program->signint_child = false;
	signal(SIGINT, ft_sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

int main(int ac, char **av, char **envp)
{
	t_shell	shell_program;

	//rl_catch_signals = 0;
    //rl_catch_sigwinch = 0;
    ((void)ac, (void)av);
    init_shell(&shell_program, envp);
    while (1)
    {
        ft_init_signals(&shell_program);
        shell_program.line = readline("Minishell> ");
        if (!shell_program.line)
			break ;
        add_history(shell_program.line);
        ft_token(shell_program.line, &shell_program);
		if (shell_program.token_list)
		{
			shell_program.ast = ft_parse(&shell_program.token_list, &shell_program);
			ft_expand_ast(shell_program.ast, &shell_program);
			dispatch_command(&shell_program, shell_program.ast);
		}
		/*
		free_token(&(shell_program.token_list));
		shell_program.token_list = NULL;
		free_ast(&shell_program);
		shell_program.ast = NULL;
		free(shell_program.line);*/
		free_all(&shell_program);
		//print_ast_debug(shell_program.ast, 0);
    }
// 恢复终端设置
    reset_terminal();
	//free_all(&shell_program);
    return 0;
}