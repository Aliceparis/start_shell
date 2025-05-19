#include "minishell.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


void ft_cd(t_shell *shell_program, char **args)
{
    char *home;
    
    if (!args[1])
    {
        home = getenv("HOME");
        if (home)
        {
            if (chdir(home) != 0)
            {
                printf("minishell: cd");
                shell_program->exit_status = 1;
            }
        }
        else
        {
            printf("minishell: cd: HOME not set\n");
            shell_program->exit_status = 1;
        }
    }
    else if (chdir(args[1]) != 0)
    {
        printf("cd erreur");
        shell_program->exit_status = 1;
    }
    shell_program->exit_status = 0;
}