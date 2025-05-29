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
        shell_program->envlist = ft_unset(shell_program->envlist, args[1]);
        shell_program->environ = env_list_to_array(shell_program->envlist);
    }
    else if (ft_strcmp(args[0], "export") == 0)
    {
        if (args[1])
        {
            i = 1;
            while (args[i])
            {
                // 尝试合并 "A=" 和 "value" 的情况
                if (ft_strchr(args[i], '=') && args[i + 1] && !ft_strchr(args[i + 1], '='))
                {
                    char *merged = ft_strjoin(args[i], args[i + 1]);
                    equal_sign = ft_strchr(merged, '=');
                    if (equal_sign)
                    {
                        *equal_sign = '\0';
                        if (is_valid_env_var_name(merged))  // 检查变量名是否合法
                        {
                            shell_program->envlist = ft_export(shell_program->envlist, merged, equal_sign + 1);
                            shell_program->environ = env_list_to_array(shell_program->envlist);
                        }
                        else
                        {
                            printf("minishell: export: `%s`: not a valid identifier\n", merged);
                            shell_program->exit_status = 1;
                        }
                        *equal_sign = '=';
                    }
                    free(merged);
                    i += 2;  // 跳过已处理的下一个参数
                }
                    // 处理普通 KEY=VALUE 格式
                else if ((equal_sign = ft_strchr(args[i], '=')))
                {
                    *equal_sign = '\0';
                    if (is_valid_env_var_name(args[i]))  // 检查变量名是否合法
                    {
                        shell_program->envlist = ft_export(shell_program->envlist, args[i], equal_sign + 1);
                        shell_program->environ = env_list_to_array(shell_program->envlist);
                    }
                    else
                    {
                        printf("minishell: export: `%s`: not a valid identifier\n", args[i]);
                         shell_program->exit_status = 1;
                    }
                    *equal_sign = '=';
                    i++;
                }
    // 处理无等号的情况（如 export VAR）
                else
                {
                    if (is_valid_env_var_name(args[i]))
                    {
                        shell_program->envlist = ft_export(shell_program->envlist, args[i], NULL);
                        shell_program->environ = env_list_to_array(shell_program->envlist);
                    }
                    else
                    {
                        printf("minishell: export: `%s`: not a valid identifier\n", args[i]);
                        shell_program->exit_status = 1;
                    }
                    i++;
                }
            }
        }
        else
            ft_env(shell_program, shell_program->envlist);
    }
}
int is_valid_env_var_name(const char *name)
{
    if (!name || !*name)
        return 0;
    
    // 首字符必须是字母或下划线
    if (!ft_isalpha(*name) && *name != '_')
        return 0;
    
    // 后续字符可以是字母、数字或下划线
    while (*++name)
    {
        if (!ft_isalnum(*name) && *name != '_')
            return 0;
    }
    
    return 1;
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

