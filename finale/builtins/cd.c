/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yujin <yujin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 15:22:45 by yujin             #+#    #+#             */
/*   Updated: 2025/06/12 16:06:19 by yujin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <limits.h>

static int	update_env_vars(t_shell *shell_program,
		const char *old_dir, const char *new_dir)
{
	if (old_dir && old_dir[0] != '\0')
	{
		shell_program->envlist = ft_export(shell_program->envlist,
				"OLDPWD", (char *)old_dir);
		if (shell_program->envlist == NULL)
			return (1);
	}
	shell_program->envlist = ft_export(shell_program->envlist,
			"PWD", (char *)new_dir);
	if (shell_program->envlist == NULL)
		return (1);
	return (0);
}

static int	change_dir_and_update_env(t_shell *shell_program,
		const char *target_path, char *prev_pwd, bool is_cd_dash)
{
	char	current_dir[PATH_MAX];
	int		status;

	status = 0;
	if (chdir(target_path) != 0)
	{
		fprintf(stderr, "minishell: cd: %s: %s\n", target_path, strerror(errno));
		return (1);
	}
	else
	{
		if (getcwd(current_dir, sizeof(current_dir)) == NULL)
		{
			fprintf(stderr, "minishell:cd: getcwd error: %s\n", strerror(errno));
			return (1);
		}
		if (is_cd_dash)
			printf("%s\n", current_dir);
		status = update_env_vars(shell_program, prev_pwd, current_dir);
		if (status != 0)
			return (error_message(shell_program,
					"minishell: cd: failed to update environment", 1), 1);
	}
	return (0);
}

static int	handle_cd_home(t_shell *shell_program, char *prev_pwd)
{
	char	*home_path;

	home_path = getenv("HOME");
	if (!home_path || *home_path == '\0')
	{
		error_message(shell_program, "minishell: cd: HOME not set", 1);
		return (1);
	}
	return (change_dir_and_update_env(shell_program,
			home_path, prev_pwd, false));
}

static int	handle_cd_oldpwd(t_shell *shell_program, char *prev_pwd)
{
	if (!prev_pwd || *prev_pwd == '\0')
	{
		error_message(shell_program, "minishell: cd: OLDPWD not set", 1);
		return (1);
	}
	return (change_dir_and_update_env(shell_program, prev_pwd, prev_pwd, true));
}

int	ft_cd(t_shell *shell_program, char **args)
{
	static char	old_pwd_static[PATH_MAX];
	int			ret_status;

	init_old_pwd(old_pwd_static, sizeof(old_pwd_static));
	ret_status = 0;
	if (!args[1] || ft_strcmp(args[1], "~") == 0)
		ret_status = handle_cd_home(shell_program, old_pwd_static);
	else if (args[2])
	{
		error_message(shell_program, "minishell: cd: too many arguments", 1);
		ret_status = 1;
	}
	else if (ft_strcmp(args[1], "-") == 0)
		ret_status = handle_cd_oldpwd(shell_program, old_pwd_static);
	else
		ret_status = change_dir_and_update_env(shell_program, args[1],
				old_pwd_static, false);
	if (ret_status == 0)
		update_old_pwd_after_cd(old_pwd_static, sizeof(old_pwd_static));
	shell_program->exit_status = ret_status;
	return (ret_status);
}
