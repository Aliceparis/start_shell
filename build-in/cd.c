/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loulou <loulou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 13:58:19 by loulou            #+#    #+#             */
/*   Updated: 2025/04/22 15:22:23 by loulou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void print_error(const char *msg)
{
    perror(msg);
}

int ft_cd(t_shell *shell, char **args)
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
                shell->exit_status = 1;
                return ;
            }
        }
        else
        {
            print_error("minishell: cd: HOME not set\n");
            shell->exit_status = 1;
            return ;
        }
    }
    else if (chdir(args[1]) != 0)
    {
        print_error("cd erreur");
        shell->exit_status = 1;
        return ;
    }
    shell->exit_status = 0;
}

