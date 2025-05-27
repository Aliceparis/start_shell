/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmen <jmen@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 14:40:38 by yujin             #+#    #+#             */
/*   Updated: 2025/05/26 12:54:17 by jmen             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_too_many_args(char **args, t_shell *shell_program)
{
	if (args[1] && args[2])
	{
		error_message(shell_program, "exit: too many arguments", 1);
		printf("exit 1!\n");
		free_all(shell_program);
		exit(1);
		return (1);
	}
	return (0);
}

int	check_if_number(char *arg, t_shell *shell_program)
{
	int	i;

	i = 0;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
		{
			printf("exit\n");
			free_all(shell_program);
			error_message(shell_program, "Ce n'est pas un nombre", 255);
			return (255);
		}
		i++;
	}
	return (0);
}

int	ft_exit(t_shell *shell_program, char **args)
{
	int	validation_result;

	validation_result = check_too_many_args(args, shell_program);
	if (validation_result != 0)
		return (validation_result);
	if (args[1])
	{
		validation_result = check_if_number(args[1], shell_program);
		if (validation_result != 0)
			exit(validation_result);
		shell_program->exit_status = (unsigned char)(ft_atoi(args[1]));
		printf("exit\n");
		free_all(shell_program);
		exit(shell_program->exit_status);
	}
	printf("exit\n");
	free_all(shell_program);
	exit(0);
}
