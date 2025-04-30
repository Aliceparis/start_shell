#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <readline/readline.h>

volatile sig_atomic_t g_heredoc_interrupted = 0;

// Gestion de SIGINT (Ctrl+C)
void sigint_handler(int sig)
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
    pipe(pipefd);

    signal(SIGINT, sigint_handler);

    while (1)
    {
        if (g_heredoc_interrupted)
        {
            close(pipefd[0]);
            close(pipefd[1]);
            return;
        }

        line = readline("> ");
        if (!line)  // Si EOF (Ctrl+D) est reçu
        {
            write(1, "EOF reçu (Ctrl+D)\n", 18);
            break;
        }

        if (strcmp(line, delimiter) == 0)
        {
            free(line);
            break;
        }

        write(pipefd[1], line, strlen(line));
        write(pipefd[1], "\n", 1);
        free(line);
    }

    close(pipefd[1]);
    dup2(pipefd[0], STDIN_FILENO);//Dupliquer le pipe dans l'entrée standard
    close(pipefd[0]);
    printf("heredoc terminé\n");
}

/*int main()
{
    char *delimiter = "EOF";

    printf("Minishell Heredoc\n");

    start_heredoc(delimiter);
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), stdin))// Lire depuis stdin
        printf("Lecture de stdin après heredoc: %s", buffer);

    return 0;
}*/

