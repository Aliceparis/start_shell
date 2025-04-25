#include "minishell.h"
t_shell shell_program;

static struct termios oldt;

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

int main(int ac, char **av, char **envp)
{
    ((void)ac, (void)av);
    init_shell(envp);
    while (1)
    {
        shell_program.line = readline("Minishell> ");
        if (shell_program.line)
        {	
            add_history(shell_program.line);
            ft_token(shell_program.line);
		    shell_program.ast = ft_parse(&shell_program.token_list);
            ft_expand_ast(shell_program.ast);
            dispatch_command(&shell_program, shell_program.ast);
        }
        else
            break ;
    }
// 恢复终端设置
    reset_terminal();
    return 0;
}