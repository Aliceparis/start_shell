/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loulou <loulou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 13:58:19 by loulou            #+#    #+#             */
/*   Updated: 2025/04/18 15:17:10 by loulou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void print_error(const char *msg)
{
    perror(msg);
}

int ft_cd(char **args)
{
    char *home;
    
    if (!args[1])
    {
        home = getenv("HOME");
        if (home)
        {
            if (chdir(home) != 0)
            {
                error("erreur de cd home");
                return (1);
            }
        }
        else
        {
            error("cd:erreur on ne trouve pas HOME\n");
            return (1);
        }
    }
    else if (chdir(args[1]) != 0)
    {
        error("cd erreur");
        return (1);
    }
    return (0);
}

