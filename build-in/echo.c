/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loulou <loulou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 13:59:06 by loulou            #+#    #+#             */
/*   Updated: 2025/04/17 14:11:41 by loulou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"

int ft_echo(char **args)
{
    int i;
    int line;

    i = 1;
    line = 1;
    if (args[i] && ft_strcmp(args[1], "-n") == 0)
    {
        line = 0;
        i++;        
    }
    while (args[i])
    {
        printf("%s", args[i]);
        if (args[i + 1])
            printf(" ");
            i++;
    }
    if (line)
        printf("\n");
    return (0);
}

int main(int argc, char **argv)
{
    (void)argc;
    ft_echo(argv);
    return (0);
}
