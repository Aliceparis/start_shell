/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yujin <yujin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 19:30:32 by yujin             #+#    #+#             */
/*   Updated: 2025/06/09 10:31:16 by yujin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <limits.h>    // For PATH_MAX
/*
static int	update_env(t_shell *shell_program, char *pre, char *new_dir)
{
	if (pre && pre[0] != '\0')
	{
		shell_program->envlist = ft_export(shell_program->envlist,
				"OLDPWD", pre);
		setenv("OLDPWD", pre, 1);
	}
	shell_program->envlist = ft_export(shell_program->envlist, "PWD", new_dir);
	setenv("PWD", new_dir, 1);
	return(0);
}

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

	if (pre[0] == '\0')
	{
		pwd = getenv("PWD");
		if (pwd)
			ft_strncpy(pre, pwd, sizeof(pre));
		else if (getcwd(pre, sizeof(pre)) == NULL)
			pre[0] = '\0';
	}
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
	if (getcwd(current_dir, sizeof(current_dir)) != NULL)
		ft_strncpy(pre, current_dir, sizeof(pre));
}*/

/**
 * @brief Updates the PWD and OLDPWD environment variables in the shell's envlist and system environment.
 * @param shell_program The shell context.
 * @param old_dir The previous working directory (to be set as OLDPWD). Can be NULL or empty string.
 * @param new_dir The new current working directory (to be set as PWD). Must be a valid path.
 * @return 0 on success, 1 on failure to update env (e.g., memory allocation for export).
 */
static int update_env_vars(t_shell *shell_program, const char *old_dir, const char *new_dir)
{
    // Update OLDPWD
    if (old_dir && old_dir[0] != '\0')
    {
        shell_program->envlist = ft_export(shell_program->envlist, "OLDPWD", (char *)old_dir);
        if (shell_program->envlist == NULL) return (1); // Export failed
        setenv("OLDPWD", old_dir, 1);
    }
    else // If old_dir is empty or NULL, ensure OLDPWD is unset or removed
    {
        // Depending on desired behavior for `cd -` where OLDPWD is unset:
        // You might call ft_unset here if you want to remove OLDPWD from the envlist
        // if the previous directory was not valid. For simplicity, we mostly set it.
    }

    // Update PWD
    shell_program->envlist = ft_export(shell_program->envlist, "PWD", (char *)new_dir);
    if (shell_program->envlist == NULL) return (1); // Export failed
    setenv("PWD", new_dir, 1);
    return (0); // Success
}

/**
 * @brief Attempts to change directory and update environment variables.
 * @param shell_program The shell context.
 * @param target_path The path to change to.
 * @param prev_pwd The previous working directory (to be used as OLDPWD).
 * @param is_cd_dash Flag to indicate if this is a 'cd -' operation.
 * @return 0 on success, 1 on failure.
 */
static int change_dir_and_update_env(t_shell *shell_program, const char *target_path,
                                    char *prev_pwd, bool is_cd_dash)
{
    char    current_dir[PATH_MAX];
    int     status = 0;

    if (chdir(target_path) != 0)
    {
        // Provide specific error messages for cd
        fprintf(stderr, "minishell: cd: %s: %s\n", target_path, strerror(errno));
        return (1); // Failure to change directory
    }
    else
    {
        // Get the new current working directory after successful chdir
        if (getcwd(current_dir, sizeof(current_dir)) == NULL)
        {
            // If getcwd fails, it means the current directory is inaccessible.
            // This is a rare but critical error after a successful chdir.
            fprintf(stderr, "minishell: cd: getcwd error: %s\n", strerror(errno));
            return (1); // Indicate failure
        }
        
        // If it was `cd -`, print the new directory path.
        if (is_cd_dash)
            printf("%s\n", current_dir);

        // Update PWD and OLDPWD
        status = update_env_vars(shell_program, prev_pwd, current_dir);
        if (status != 0)
        {
            // If updating env vars failed (e.g., malloc), it's a critical error
            error_message(shell_program, "minishell: cd: failed to update environment", 1);
            return (1);
        }
    }
    return (0); // Success
}

/**
 * @brief Handles 'cd' command without arguments (i.e., 'cd' or 'cd ~').
 * @param shell_program The shell context.
 * @param prev_pwd The previous working directory.
 * @return 0 on success, 1 on failure.
 */
static int handle_cd_home(t_shell *shell_program, char *prev_pwd)
{
    char    *home_path;

    home_path = getenv("HOME");
    if (!home_path || *home_path == '\0')
    {
        error_message(shell_program, "minishell: cd: HOME not set", 1);
        return (1);
    }
    return (change_dir_and_update_env(shell_program, home_path, prev_pwd, false));
}

/**
 * @brief Handles 'cd -' command.
 * @param shell_program The shell context.
 * @param prev_pwd The previous working directory (which will become the new PWD).
 * @return 0 on success, 1 on failure.
 */
static int handle_cd_oldpwd(t_shell *shell_program, char *prev_pwd)
{
    if (!prev_pwd || *prev_pwd == '\0')
    {
        error_message(shell_program, "minishell: cd: OLDPWD not set", 1);
        return (1);
    }
    // Call change_dir_and_update_env, indicating it's a 'cd -' operation
    return (change_dir_and_update_env(shell_program, prev_pwd, prev_pwd, true));
}

/**
 * @brief Implements the 'cd' built-in command.
 * Changes the current working directory and updates PWD/OLDPWD environment variables.
 * @param shell_program The shell context.
 * @param args The arguments array for the 'cd' command.
 * @return 0 on success, 1 on failure.
 */
int ft_cd(t_shell *shell_program, char **args)
{
    // Use a static buffer for `pre` to store the previous directory,
    // as it needs to persist across `cd` calls.
    static char old_pwd_static[PATH_MAX];
    int         ret_status = 0; // Initialize return status to success

    // Initialize old_pwd_static on first run or if it's explicitly empty.
    // This serves as the OLDPWD for the *next* cd command.
    if (old_pwd_static[0] == '\0')
    {
        const char *pwd_env = getenv("PWD");
        if (pwd_env)
            ft_strncpy(old_pwd_static, pwd_env, sizeof(old_pwd_static) - 1);
        else if (getcwd(old_pwd_static, sizeof(old_pwd_static) - 1) == NULL)
            old_pwd_static[0] = '\0'; // Set to empty if getcwd fails
        old_pwd_static[sizeof(old_pwd_static) - 1] = '\0'; // Ensure null-termination
    }

    // Case 1: No arguments or "cd ~" (handle HOME)
    if (!args[1] || ft_strcmp(args[1], "~") == 0)
    {
        ret_status = handle_cd_home(shell_program, old_pwd_static);
    }
    // Case 2: Too many arguments
    else if (args[2])
    {
        error_message(shell_program, "minishell: cd: too many arguments", 1);
        ret_status = 1;
    }
    // Case 3: "cd -" (handle OLDPWD)
    else if (ft_strcmp(args[1], "-") == 0)
    {
        ret_status = handle_cd_oldpwd(shell_program, old_pwd_static);
    }
    // Case 4: Specific path provided
    else
    {
        ret_status = change_dir_and_update_env(shell_program, args[1], old_pwd_static, false);
    }

    // If the command was successful (ret_status == 0),
    // update the static `old_pwd_static` to the *current* PWD for the next call.
    // This must happen after the chdir and env updates of the current command are finished.
    if (ret_status == 0)
    {
        char current_actual_pwd[PATH_MAX];
        if (getcwd(current_actual_pwd, sizeof(current_actual_pwd)) != NULL)
        {
            ft_strncpy(old_pwd_static, current_actual_pwd, sizeof(old_pwd_static) - 1);
            old_pwd_static[sizeof(old_pwd_static) - 1] = '\0'; // Ensure null-termination
        }
        else
        {
            // If getcwd fails here (after successful chdir), it's a critical state.
            // Shell might be in a detached directory. Handle by setting OLDPWD to empty
            // or a known bad state for the next command.
            old_pwd_static[0] = '\0'; // Clear OLDPWD for next command if getcwd fails
            fprintf(stderr, "minishell: cd: warning: current directory inaccessible after cd\n");
            // Don't change ret_status to 1, as the cd itself was successful.
            // This is more of a warning for future operations.
        }
    }
    // Set the shell's exit status based on the command's success/failure.
    shell_program->exit_status = ret_status;
    return (ret_status);
}