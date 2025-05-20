/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yujin <yujin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 14:29:09 by yujin             #+#    #+#             */
/*   Updated: 2025/05/20 13:17:10 by yujin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		printf("exit 1!\n");
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
		//free_all(shell_program);
        exit(shell_program->exit_status);
    }
    else if(args[1])
        shell_program->exit_status = ft_atoi(args[1]);
    else
	{
        shell_program->exit_status = 0;
	}
	shell_program->token_list = NULL;
	shell_program->ast = NULL;
	shell_program->envlist = NULL;
    exit(0);
}
