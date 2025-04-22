#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include "tokenising.h"
# include "parsing.h"

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <termios.h>

# include <readline/history.h>
# include <readline/readline.h>

typedef struct s_env
{
    char    *key;
    char    *value;
    struct s_env    *next;
}t_env;

typedef struct s_shell
{
    char    *line;
    t_token *token_list;
	t_token	*current_token;
    t_env   *envlist;
    int stdin;
    int stdout;
    char    **environ;
    //?? error de flux ;
    struct termios  termios_p;
	ASTnode	*ast;
    int exit_status;//退出值
}t_shell;

externe t_shell shell_program;

typedef

# endif
