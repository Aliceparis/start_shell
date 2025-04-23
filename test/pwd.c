#include "libft.h"
#include "minishell.h"

#define PATH_MAX 1024

void error(const char *msg, int n_exit)
{
    perror(msg);
    exit(n_exit);
}


void ft_pwd(t_shell *shell_program)
{
    char cwd[PATH_MAX];

    if (getcwd(cwd, PATH_MAX) != NULL)
        printf("%s\n", cwd);
    else
        error("pwd non trouve", 1);
    shell_program->exit_status = 0;
}

/*int main(void)
{
    ft_pwd();
    return (0);
}*/