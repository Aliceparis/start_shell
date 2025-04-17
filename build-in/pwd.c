/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loulou <loulou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 13:58:52 by loulou            #+#    #+#             */
/*   Updated: 2025/04/17 14:31:41 by loulou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"

void error(const char *msg, int n_exit)
{
    perror(msg);
    exit(n_exit);
}


int ft_pwd(void)
{
    char cwd[1024];

    if (getcwd(cwd,1024) != NULL)
        printf("%s\n", cwd);
    else
        error("pwd non trouve", 1);
    return (0);
}

int main(void)
{
    ft_pwd();
    return (0);
}