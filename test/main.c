#include "minishell.h"

void    init_shell(char **env)
{
    ft_memset(&shell_program, 0, sizeof(t_shell));
    shell_prgram.environ = env;
    init_envlist();
    shell_program.stdin = dup(0);
    shell_program.stdout = dup2(1);
    tcgettattr(STDIN_FILENO, &shell_program.termios_p);
}

int main(int ac, char **av, char **envp)
{
    ((void)ac, (void)av);
    init_shell(envp);
    while (1)
    {
        shell_program.line = readline(Minishell);
       if (shell_program.line)
        	add_history(shell_program.line);
        else
        /////fonctions de clear ???
        ft_tokenise(shell_program.line);
		shell_program.ast = ft_parse();

    }
}