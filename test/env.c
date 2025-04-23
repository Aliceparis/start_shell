#include "minishell.h"

void ft_env(t_shell *shell_program, t_env *env)
{
    while (env)
    {
        printf("%s=%s\n", env->key, env->value);
        env = env->next;
    }
    shell_program->exit_status = 0;
}