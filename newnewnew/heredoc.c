#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <readline/readline.h>
#include "include/minishell.h"

volatile sig_atomic_t g_heredoc_interrupted = 0;

// Gestion de SIGINT (Ctrl+C)
static void sigint_handler(int sig)
{
    (void)sig;
    g_heredoc_interrupted = 1;
    write(1, "\nheredoc interrupted\n", 21);
}

// Fonction pour lancer le heredoc et lire jusqu'à ce que le délimiteur soit trouvé
void start_heredoc(const char *delimiter)
{
    char *line;
    int pipefd[2];

    g_heredoc_interrupted = 0;
    signal(SIGINT, sigint_handler);

    if (pipe(pipefd) == -1)
        return ;

    while (1)
    {
        if (g_heredoc_interrupted)
        {
            close(pipefd[0]);
            close(pipefd[1]);
            return ;
        }
        line = readline("> ");
        if (!line)
            break;
        if (ft_strcmp(line, delimiter) == 0)
        {
            free(line);
            break;
        }
        write(pipefd[1], line, strlen(line));
        write(pipefd[1], "\n", 1);
        free(line);
    }

    close(pipefd[1]);
    return (reset_terminal());
}


