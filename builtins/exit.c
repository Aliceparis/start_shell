/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmen <jmen@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 19:31:38 by yujin             #+#    #+#             */
/*   Updated: 2025/06/14 23:34:56 by jmen             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_too_many_args(char **args, t_shell *shell_program)
{
	if (args[1] && args[2])
	{
		free_all(shell_program);
		error_message(shell_program, "exit: too many arguments", 1);
		return (1);
	}
	return (0);
}

int	check_if_number(char *arg, t_shell *shell_program)
{
	int	i;

	i = 0;
	if (!arg)
	{
		shell_program->exit_status = 1;
		return (0);
	}
	if (arg[0] == '+' || arg[0] == '-')
		i++;
	if (arg[i] == '\0')
		return (0);
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return (0);
		i++;
	}
	return (1);
}

int	ft_exit(t_shell *shell_program, char **args)
{
	printf("exit\n");
	if (!args[1])
	{
		free_all(shell_program);
		exit(0);
	}
	if (check_if_number(args[1], shell_program) == 0)
	{
		free_all(shell_program);
		error_message(shell_program, "exit: numeric argument required", 255);
		exit(255);
	}
	if (check_too_many_args(args, shell_program))
		return (1);
	shell_program->exit_status = (unsigned char)(ft_atoi(args[1]));
	free_all(shell_program);
	exit(shell_program->exit_status);
}
