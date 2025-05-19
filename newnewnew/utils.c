#include "minishell.h"

void error_message(t_shell *shell_program, const char *msg, int exit_s)
{
    ft_fprintf(stderr, "%s\n",msg);
    shell_program->exit_status = exit_s;
}

