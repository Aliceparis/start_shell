/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yujin <yujin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 19:30:55 by yujin             #+#    #+#             */
/*   Updated: 2025/06/14 11:31:09 by yujin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_n_option(const char *s)
{
	int	i;

	i = 1;
	if (!s || s[0] != '-')
		return (0);
	if (s[1] == '\0')
		return (0);
	while (s[i])
	{
		if (s[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int	ft_echo(t_shell *shell_program, char **args)
{
	int	i;
	int	line;

	i = 1;
	line = 1;
	(void)shell_program;
	while (args[i] && is_n_option(args[i]))
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
	shell_program->exit_status = 0;
	return (0);
}
