#include "minishell.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#ifndef BUFF 
#define BUFF 1024
#endif

void ft_cd(t_shell *shell_program, char **args)
{
    char *home;
    static char pre[BUFF] = {0};
    char current_dir[BUFF];
    char new_dir[BUFF];

    if (getcwd(current_dir, sizeof(current_dir)) == NULL)
    {
        strcpy(current_dir, getenv("PWD") ? getenv("PWD") : "");
        shell_program->exit_status = 1;
    }
    if (!args[1])
    {
        home = getenv("HOME");
        if (!home)
        {
            printf("minishell: cd: HOME not set\n");
            shell_program->exit_status = 1;
            return ;
        }
        if (chdir(home) != 0)
        {
            perror("minishell: cd");
            shell_program->exit_status = 1;
            return ;
        }
    }
    else if (args[2])
    {
        printf("minishell: cd: too many arguments\n");
        shell_program->exit_status = 1;
        return;
    }
    else if (ft_strcmp(args[1], "-") == 0)
    {
        if (pre[0] == '\0')
        {
            printf("minishell: cd: OLDPWD not set\n");
            shell_program->exit_status = 1;
            return;
        }
        if (chdir(pre) != 0)
        {
            perror("minishell: cd -");
            shell_program->exit_status = 1;
            return;
        }
    }
    else if (chdir(args[1]) != 0)
    {
        perror("minishell: cd");
        shell_program->exit_status = 1;
        return;
    }
    ft_strncpy(pre, current_dir, sizeof(pre));
    if (getcwd(new_dir, sizeof(new_dir)) != NULL)
    {
        shell_program->envlist = ft_export(shell_program->envlist, "OLDPWD", pre);
        shell_program->envlist = ft_export(shell_program->envlist, "PWD", new_dir);
    }
    else
    {
        shell_program->envlist = ft_export(shell_program->envlist, "OLDPWD", pre);
        shell_program->envlist = ft_export(shell_program->envlist, "PWD", args[1] ? args[1] : home);
        perror("getcwd");
    }
    shell_program->exit_status = 0;
}
