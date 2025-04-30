#include "libft.h"
#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int ft_exit(t_shell *shell_program, char **args)
{
    int i;

    i = 0;
    if (args[1] && args[2])
    {
        error_message(shell_program, "exit: too many arguments", 1);
        return (1);
    }
    else if (args[1])
    {
        while (args[1][i])
        {
            if(!ft_isdigit(args[1][i]))
            {
                error_message(shell_program, "Ce n'est pas un nombre", 255);
                exit(255);
            }
            i++;
        }
        shell_program->exit_status = (unsigned char)(ft_atoi(args[1]));
		printf("sortie in ft_exit\n");
		free_all(shell_program);
        exit(shell_program->exit_status);
    }
    else if(args[1])
        shell_program->exit_status = ft_atoi(args[1]);
    else
	{
        shell_program->exit_status = 0;
	}
	free_all(shell_program);
    exit(0);
}