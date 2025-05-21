#include "include/minishell.h"

volatile sig_atomic_t g_heredoc_interrupted = 0;

// Gestion de SIGINT (Ctrl+C)
static void sigint_handler(int sig)
{
    (void)sig;
    g_heredoc_interrupted = 1;
    //write(1, "^C\n", 3);
    rl_done = 1;
}

void heredoc_loop(const char *delimiter, int write_fd)
{
    char *line;

    while (1)
    {
        if (g_heredoc_interrupted == 1)
        {
            close(write_fd);
            exit(130); // code pour signal INT
        }
        line = readline("> ");
        if (!line)
            break;
        if (ft_strcmp(line, delimiter) == 0)
        {
            free(line);
            break;
        }
        write(write_fd, line, strlen(line));
        write(write_fd, "\n", 1);
        free(line);
    }
    close(write_fd);
    exit(0);
}

// Fonction pour lancer le heredoc et lire jusqu'à ce que le délimiteur soit trouvé
void start_heredoc(const char *delimiter)
{
    int pipefd[2];
    pid_t pid;
    int status;
    char buffer[1024];
    ssize_t bytes_read;

    if (pipe(pipefd) == -1)
        return ;
    pid = fork();
    if (pid == -1)
        return ;
    if (pid == 0)
    {
        signal(SIGINT, sigint_handler); // gérer Ctrl+C
        rl_catch_signals = 0;
        close(pipefd[0]); // l'enfant écrit
        heredoc_loop(delimiter, pipefd[1]);
    }
    else
    {
        close(pipefd[1]);
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
        {
            g_heredoc_interrupted = 0;
            close(pipefd[0]);
            reset_terminal();
            return ;
        }
        while ((bytes_read = read(pipefd[0], buffer, sizeof(buffer) - 1)) > 0)
        {
            buffer[bytes_read] = '\0';
            write(1, buffer, bytes_read);
        }
        close(pipefd[0]);
    }
    reset_terminal();
}
