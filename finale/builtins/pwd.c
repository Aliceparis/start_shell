/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yujin <yujin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 19:31:18 by yujin             #+#    #+#             */
/*   Updated: 2025/06/12 15:03:52 by yujin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_pwd(t_shell *shell_program)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, PATH_MAX) != NULL)
	{
		printf("%s\n", cwd);
		return (0);
	}
	else
	{
		printf("pwd non trouve");
		shell_program->exit_status = 1;
		return (1);
	}
}

/*int main(void)
{
	ft_pwd();
	return (0);
}*/
