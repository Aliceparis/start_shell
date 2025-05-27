#include "../include/minishell.h"

#define PATH_MAX 1024

void ft_pwd(t_shell *shell_program)
{
    char cwd[PATH_MAX];

    if (getcwd(cwd, PATH_MAX) != NULL)
        printf("%s\n", cwd);
    else
    {
        printf("pwd non trouve");
        shell_program->exit_status = 1;
    }
}

/*int main(void)
{
    ft_pwd();
    return (0);
}*/