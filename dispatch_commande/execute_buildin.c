/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_buildin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yujin <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 11:58:43 by yujin             #+#    #+#             */
/*   Updated: 2025/06/14 11:58:44 by yujin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_export_key_value(t_shell *shell_program,
		char *key_value, char *equal_sign)
{
	*equal_sign = '\0';
	if (is_valid_env_var_name(key_value))
	{
		shell_program->envlist = ft_export(shell_program->envlist,
				key_value, equal_sign + 1);
		shell_program->environ = env_list_to_array(shell_program->envlist);
	}
	else
	{
		printf("minishell: export: `%s`: not a valid identifier\n", key_value);
		shell_program->exit_status = 1;
	}
	*equal_sign = '=';
}

/* Vérifie si name est un nom valide pour une variable d'environnement.
* return : 1 ok, 0 faux*/
// Sous-fonction : traite un argument contenant un '='
static int	handle_export_assignment(t_shell *shell_program,
			char *key, char *value)
{
	if (is_valid_env_var_name(key))
	{
		shell_program->envlist = ft_export(shell_program->envlist, key, value);
		shell_program->environ = env_list_to_array(shell_program->envlist);
		if (shell_program->exit_status != 1)
			shell_program->exit_status = 0;
		return (0);
	}
	else
	{
		printf("Export : '%s' not a valid identifier\n", key);
		shell_program->exit_status = 1;
		return (1);
	}
}

// Fonction principale
int	handle_export_arg(t_shell *shell_program, char *arg)
{
	char	*equal_sign;
	char	*key;
	char	*value;
	int		arg_status;

	arg_status = 0;
	equal_sign = ft_strchr(arg, '=');
	if (equal_sign)
	{
		*equal_sign = '\0';
		key = arg;
		value = equal_sign + 1;
		arg_status = handle_export_assignment(shell_program, key, value);
		*equal_sign = '=';
	}
	return (arg_status);
}

int	is_valid_env_var_name(const char *name)
{
	if (!name || !*name)
		return (0);
	if (!ft_isalpha(*name) && *name != '_')
		return (0);
	while (*++name)
	{
		if (!ft_isalnum(*name) && *name != '_')
			return (0);
	}
	return (1);
}

int	excute_builtin(t_shell *shell_program, char **args)
{
	int	i;
	int	final_status;

	final_status = 0;
	if (!args || !args[0])
		return (1);
	if (ft_strcmp(args[0], "export") == 0)
	{
		i = 1;
		while (args[i])
		{
			if (handle_export_arg(shell_program, args[i]) != 0)
				final_status = 1;
			i++;
		}
		return (final_status);
	}
	else
		return (execute_builtin_simple(shell_program, args));
}
