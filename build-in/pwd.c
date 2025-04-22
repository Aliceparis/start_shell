/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loulou <loulou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 13:58:52 by loulou            #+#    #+#             */
/*   Updated: 2025/04/22 15:37:42 by loulou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "../minishell.h"

#define PATH_MAX 1024

void error(const char *msg, int n_exit)
{
    perror(msg);
    exit(n_exit);
}


void ft_pwd(t_shell *shell)
{
    char cwd[PATH_MAX];

    if (getcwd(cwd, PATH_MAX) != NULL)
        printf("%s\n", cwd);
    else
        error("pwd non trouve", 1);
    shell->exit_status = 0;
}

/*int main(void)
{
    ft_pwd();
    return (0);
}*/