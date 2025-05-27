#include "minishell.h"
t_shell shell_program;
volatile sig_atomic_t g_signal = 0;

//static struct termios oldt;

void    init_shell(t_shell *shell_program, char **env)
{
    shell_program->environ = env;
	shell_program->envlist = NULL;
	shell_program->token_list = NULL;
	shell_program->ast = NULL;
	shell_program->exit_status = 0;
	init_envlist(shell_program);
	shell_program->stdin = dup(STDIN_FILENO);
    shell_program->stdout = dup(STDOUT_FILENO);
    //shell_program->heredoc_sigint = false;

    tcgetattr(STDIN_FILENO, &shell_program->oldt);
    //tcsetattr(STDIN_FILENO, TCSANOW, &shell_program->oldt);
}
void    ft_init_signals(t_shell *shell_program)
{
    struct sigaction    sa;

    sa.sa_handler = handle_signal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    g_signal = NORMAL;

    struct termios newt;
     newt = shell_program->oldt;
    newt.c_lflag &= ~(ECHOCTL);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    sigaction(SIGINT, &sa, NULL);
    signal(SIGQUIT, SIG_IGN);
    signal(SIGPIPE, SIG_IGN);
}

void    handle_signal(int sig)
{
    (void)sig;
    if (g_signal == HEREDOC)
    {
        write(STDERR_FILENO, "\n", 1);
        exit(1);
    }
    if (g_signal == PIPE)
    {
        write(STDERR_FILENO, "\n", 1);
        return; // 让父进程继续处理
    }
    if (g_signal == NORMAL)
    {
        write(STDOUT_FILENO, "\n", 1);  // 换行
        rl_on_new_line();  // 通知 readline 已换行
        rl_replace_line("", 0);  // 清空当前输入行
        rl_redisplay();
    }
}
/*void reset_signals_in_child(void)
{
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
    signal(SIGTSTP, SIG_DFL);
}*/
void    reset_terminal(t_shell *shell_program)
{
    tcsetattr(STDIN_FILENO, TCSANOW, &shell_program->oldt);
}
int main(int ac, char **av, char **envp)
{
	t_shell	shell_program;

    ((void)ac, (void)av);
    init_shell(&shell_program, envp);
    ft_init_signals(&shell_program);
    while (1)
    {
        
        shell_program.line = readline("Minishell> ");
        if (!shell_program.line)
			break ;
        add_history(shell_program.line);
        ft_token(shell_program.line, &shell_program);
		if (shell_program.token_list)
		{
			shell_program.ast = ft_parse(&shell_program.token_list, &shell_program);
            if (shell_program.ast)
            {
			    ft_expand_ast(shell_program.ast, &shell_program);
                dispatch_command(&shell_program, shell_program.ast);
            }
		}
		//free_all(&shell_program);
    }
    free_all(&shell_program);
// 恢复终端设置
    reset_terminal(&shell_program);
    return 0;
}
