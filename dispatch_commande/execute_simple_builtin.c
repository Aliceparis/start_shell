/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_simple_builtin.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yujin <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 11:55:47 by yujin             #+#    #+#             */
/*   Updated: 2025/06/14 11:55:49 by yujin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *cmd)
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

int	execute_builtin_simple(t_shell *shell_program, char **args)
{
	int	i;

	i = 1;
	if (ft_strcmp(args[0], "exit") == 0)
		return (ft_exit(shell_program, args));
	else if (ft_strcmp(args[0], "pwd") == 0)
		return (ft_pwd(shell_program));
	else if (ft_strcmp(args[0], "echo") == 0)
		return (ft_echo(shell_program, args));
	else if (ft_strcmp(args[0], "cd") == 0)
		return (ft_cd(shell_program, args));
	else if (ft_strcmp(args[0], "env") == 0)
		return (ft_env(shell_program, shell_program->envlist));
	else if (ft_strcmp(args[0], "unset") == 0)
	{
		while (args[i])
		{
			shell_program->envlist = ft_unset(shell_program->envlist, args[i]);
			shell_program->environ = env_list_to_array(shell_program->envlist);
			i++;
		}
		return (0);
	}
	return (1);
}
