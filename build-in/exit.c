/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loulou <loulou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 13:59:02 by loulou            #+#    #+#             */
/*   Updated: 2025/04/18 15:52:25 by loulou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "../minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int ft_exit(t_shell *shell,char **args)
{
    if (args[1] && args[2])
    {
        shell->ret = 1;
        error("exit: too many arguments\n");
        return ;
    }
    else if (args[1] && ft_atoi(args[1]) == 0)
    {
        shell->ret = 255;
        exit(shell->ret);
    }
    else if(args[1])
        shell->rest = ft_atoi(args[1]);
    else
        shell->ret = 0;
    exit(shell->ret);
}