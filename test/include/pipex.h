#ifndef PIPEX_H
# define PIPEX_H

# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <stdio.h>
# include <sys/wait.h>
# include <fcntl.h>
# include "../libft/libft.h"

//void	error(char *msg, int status);
char	*find_path(char *cmd, char **envp);
void	execute(char *argv, char **envp);

/*int		get_next_line(char **line);*/
#endif