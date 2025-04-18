/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loulou <loulou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 15:59:47 by loulou            #+#    #+#             */
/*   Updated: 2025/04/18 16:24:08 by loulou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_builtin(char *cmd)
{
    if (!cmd)
        return (0);
    if (ft_strcmp(cmd, "exit") == 0)
        return (1);
    if (ft_strcmp(cmd, "pwd") == 0)
        return (1);
    if (ft_strcmp(cmd, "echo") == 0)
        return (1);
    if (ft_strcmp(cmd, "cd") == 0)
        return (1);
    if (ft_strcmp(cmd, "env") == 0)
        return (1);
    if (ft_strcmp(cmd, "unset") == 0)
        return (1);
    if (ft_strcmp(cmd, "export") == 0)
        return (1);
    return (0);
}

int excute_builtin(t_shell *shell, char **args)
{
    char *equal_sign;
    if (!args[0])
        return (1);
    if (ft_strcmp(args[0], "exit") == 0)
        ft_exit(args);
    else if (ft_strcmp(args[0], "pwd") == 0)
        ft_pwd();
    else if (ft_strcmp(args[0], "echo") == 0)
        ft_echo(args);
    else if (ft_strcmp(args[0], "cd") == 0)
        return (ft_cd(args, shell->envlist));
    else if (ft_strcmp(args[0], "env") == 0)
        ft_env(shell->envlist);
    else if (ft_strcmp(args[0], "unset") == 0)
        shell->envlist = ft_unset(shell->envlist, args[1]);
    else if (ft_strcmp(args[0], "export") == 0)
    {
        if (args[1])
        {
            equal_sign = ft_strchr(args[1], '=');
            if (equal_sign)
            {
                *equal_sign = '\0';
                shell->envlist = ft_export(shell->envlist, args[1], (equal_sign + 1));
                *equal_sign = '=';
            }
            else
                error("minishell: export: invalid format (expected VAR=VALUE)\n");
        }
        else
            ft_env(shell->envlist);
    }
    return (0);
}
