#include "libft.h"
#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int ft_exit(t_shell *shell_program,char **args)
{
    int i;

    i = 0;
    if (args[1] && args[2])
    {
        shell_program->exit_status = 1;
        error("exit: too many arguments\n", 1);
        return (1);
    }
    else if (args[1])
    {
        while (args[1][i])
        {
            if(!ft_isdigit(args[1][i]))
            {
                error("ce n'est pas un nombre", 255);
                exit(255);
            }
            i++;
        }
        shell_program->exit_status = (unsigned char)(ft_atoi(args[1]));
        exit(shell_program->exit_status);
    }
    else if(args[1])
        shell_program->exit_status = ft_atoi(args[1]);
    else
        shell_program->exit_status = 0;
    exit(0);
}