/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_end.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yujin <yujin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 16:06:54 by yujin             #+#    #+#             */
/*   Updated: 2025/06/13 09:53:05 by yujin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_old_pwd(char *old_pwd_static, size_t size)
{
	const char	*pwd_env;

	if (old_pwd_static[0] != '\0')
		return ;
	pwd_env = getenv("PWD");
	if (pwd_env)
		ft_strncpy(old_pwd_static, pwd_env, size - 1);
	else if (getcwd(old_pwd_static, size - 1) == NULL)
		old_pwd_static[0] = '\0';
	old_pwd_static[size - 1] = '\0';
}

void	update_old_pwd_after_cd(char *old_pwd_static, size_t size)
{
	char	current_actual_pwd[PATH_MAX];

	if (getcwd(current_actual_pwd, sizeof(current_actual_pwd)) != NULL)
	{
		ft_strncpy(old_pwd_static, current_actual_pwd, size - 1);
		old_pwd_static[size - 1] = '\0';
	}
	else
	{
		old_pwd_static[0] = '\0';
		fprintf(stderr,
			"minishell: cd: warning: current directory inaccessible after cd\n");
	}
}
