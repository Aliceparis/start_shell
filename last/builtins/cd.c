/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yujin <yujin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 14:16:25 by yujin             #+#    #+#             */
/*   Updated: 2025/05/24 15:36:10 by yujin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	update_env(t_shell *shell_program, char *pre, char *new_dir)
{
	shell_program->envlist = ft_export(shell_program->envlist, "OLDPWD", pre);
	shell_program->envlist = ft_export(shell_program->envlist, "PWD", new_dir);
}

// Fonction pour gérer le répertoire HOME
static void	handle_home(t_shell *shell_program)
{
	char	*home;
	char	current_dir[BUFF];

	home = getenv("HOME");
	if (!home)
		error_message(shell_program, "minishell: cd: HOME not set", 1);
	else
	{
		if (chdir(home) != 0)
			error_message(shell_program, "minishell: cd", 1);
		else
		{
			if (getcwd(current_dir, sizeof(current_dir)) != NULL)
				update_env(shell_program, "", current_dir);
		}
	}
}

static void	handle_oldpwd(char *pre, t_shell *shell_program)
{
	char	current_dir[BUFF];

	if (pre[0] == '\0')
		error_message(shell_program, "minishell: cd: OLDPWD not set", 1);
	else
	{
		if (chdir(pre) != 0)
			error_message(shell_program, "minishell: cd", 1);
		else
		{
			if (getcwd(current_dir, sizeof(current_dir)) != NULL)
				update_env(shell_program, pre, current_dir);
		}
	}
}

static void	process_cd_args(t_shell *shell_program, char **args, char *pre)
{
	char	current_dir[BUFF];

	if (!args[1])
		handle_home(shell_program);
	else if (args[2])
		error_message(shell_program, "minishell: cd: too many arguments", 1);
	else if (ft_strcmp(args[1], "-") == 0)
		handle_oldpwd(pre, shell_program);
	else
	{
		if (chdir(args[1]) != 0)
			error_message(shell_program, "minishell: cd: no such file", 1);
		else
		{
			if (getcwd(current_dir, sizeof(current_dir)) != NULL)
				update_env(shell_program, pre, current_dir);
		}
	}
}

void	ft_cd(t_shell *shell_program, char **args)
{
	static char	pre[BUFF];
	char		current_dir[BUFF];
	const char	*pwd;

	ft_memset(pre, 0, sizeof(pre));
	pwd = getenv("PWD");
	if (getcwd(current_dir, sizeof(current_dir)) == NULL)
	{
		if (pwd)
			ft_strcpy(current_dir, pwd);
		else
			ft_strcpy(current_dir, "");
	}
	process_cd_args(shell_program, args, pre);
	if (shell_program->exit_status == 1)
		return ;
	ft_strncpy(pre, current_dir, sizeof(pre));
}
