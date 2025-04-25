#include "minishell.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void print_error(const char *msg)
{
    perror(msg);
}

int ft_cd(t_shell *shell_program, char **args)
{
    char *home;
    
    if (!args[1])
    {
        home = getenv("HOME");
        if (home)
        {
            if (chdir(home) != 0)
            {
                print_error("minishell: cd");
                shell_program->exit_status = 1;
                return (shell_program->exit_status);
            }
        }
        else
        {
            print_error("minishell: cd: HOME not set\n");
            shell_program->exit_status = 1;
            return (shell_program->exit_status);
        }
    }
    else if (chdir(args[1]) != 0)
    {
        print_error("cd erreur");
        shell_program->exit_status = 1;
        return (shell_program->exit_status);
    }
    shell_program->exit_status = 0;
    return (shell_program->exit_status);
}