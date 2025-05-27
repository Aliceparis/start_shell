#include "minishell.h"

void excute_builtin(t_shell *shell_program, char **args)
{
    char *equal_sign;
    int i;

    if (!args[0])
        return ;
    if (ft_strcmp(args[0], "exit") == 0)
        ft_exit(shell_program, args);
    else if (ft_strcmp(args[0], "pwd") == 0)
        ft_pwd(shell_program);
    else if (ft_strcmp(args[0], "echo") == 0)
        ft_echo(shell_program, args);
    else if (ft_strcmp(args[0], "cd") == 0)
        ft_cd(shell_program, args);
    else if (ft_strcmp(args[0], "env") == 0)
        ft_env(shell_program, shell_program->envlist);
    else if (ft_strcmp(args[0], "unset") == 0)
    {
        i = 1;
        while (args[i] != NULL)
        {
            shell_program->envlist = ft_unset(shell_program->envlist, args[i]);
            i++;
        }
        shell_program->environ = env_list_to_array(shell_program->envlist);
    }
    else if (ft_strcmp(args[0], "export") == 0)
    {
        if (args[1])
        {
            i = 1;
            while (args[i])
            {
                printf("ars dans export :%s\n", args[i]);
                equal_sign = ft_strchr(args[i], '=');
                if (equal_sign)
                {
                    *equal_sign = '\0';
                    shell_program->envlist = ft_export(shell_program->envlist, args[i], (equal_sign + 1));
                    *equal_sign = '=';
                    shell_program->environ = env_list_to_array(shell_program->envlist);
                }
                else
                {
                    printf("minishell: export: invalid format\n");
                    shell_program->exit_status = 1;
                }
                i++;
            }
        }
        else
            ft_env(shell_program, shell_program->envlist);
    }
}


int is_builtin(char *cmd)
{
    if (!cmd)
        return (0);
    if (ft_strcmp(cmd, "exit") == 0)
        return (1);
    if (ft_strcmp(cmd, "pwd") == 0)
        return (1);
    if (ft_strcmp(cmd, "echo") == 0)
        return (1);
    if (ft_strcmp(cmd, "cd") == 0)
        return (1);
    if (ft_strcmp(cmd, "env") == 0)
        return (1);
    if (ft_strcmp(cmd, "unset") == 0)
        return (1);
    if (ft_strcmp(cmd, "export") == 0)
        return (1);
    return (0);
}

