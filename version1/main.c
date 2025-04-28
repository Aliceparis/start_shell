#include "minishell.h"
t_shell shell_program;

static struct termios oldt;

static void	ft_sigint_handler(int num)
{
	(void)num;
	if (shell_program.signint_child)
	{
		ft_putstr_fd("\n", 1);
		shell_program.signint_child = false;
		shell_program.heredoc_sigint = true;
	}
	else
	{
		ft_putstr_fd("\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}
void    init_shell(char **env)
{
    ft_memset(&shell_program, 0, sizeof(t_shell));
    shell_program.environ = env;
    init_envlist();
    shell_program.stdin = dup(0);
    shell_program.stdout = dup(1);
    tcgetattr(STDIN_FILENO, &shell_program.oldt);
}

void reset_terminal(void)
{
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // 恢复原始设置
}
void	ft_init_signals(void)
{
	struct termios	term;

	term = shell_program.oldt;
	term.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
	shell_program.heredoc_sigint = false;
	shell_program.signint_child = false;
	signal(SIGINT, ft_sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

int main(int ac, char **av, char **envp)
{
    ((void)ac, (void)av);
    init_shell(envp);
    while (1)
    {
        ft_init_signals();
        shell_program.line = readline("Minishell> ");
        if (shell_program.line)
        {	
            add_history(shell_program.line);
            ft_token(shell_program.line, &(shell_program.token_list));
		    shell_program.ast = ft_parse(&shell_program.token_list);
			//print_ast(shell_program.ast, 0);
			//printf("after expansion\n");
            ft_expand_ast(shell_program.ast);
			//printf("after expansion\n");
            //print_ast(shell_program.ast, 0);
            dispatch_command(&shell_program, shell_program.ast);
        }
        else
            break ;///fonction free and error ?????
    }
// 恢复终端设置
    reset_terminal();
    return 0;
}