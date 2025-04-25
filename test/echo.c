#include "libft.h"
#include "minishell.h"

void ft_echo(t_shell *shell_program, char **args)
{
    int i;
    int line;

    i = 1;
    line = 1;
    if (args[i] && ft_strcmp(args[i], "-n") == 0)
    {
        line = 0;
        i++;    
    }
    while (args[i])
    {
        printf("%s", args[i]);
        if (args[i + 1])
            printf(" ");
        i++;
    }
    if (line)
        printf("\n");
    shell_program->exit_status = 0;
}
int ft_strcmp(const char *s1, const char *s2)
{
    int i = 0;

    while (s1[i] && s2[i] && s1[i] == s2[i])
        i++;
    return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}
/*int main(int argc, char **argv)
{
    (void)argc;
    ft_echo(argv);
    return (0);
}*/