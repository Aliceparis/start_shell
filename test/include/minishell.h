#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include "tokenising.h"
# include "parsing.h"
# include "ft_fprintf.h"

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <termios.h>
# include <signal.h>
# include <stdbool.h>

# include <readline/history.h>
# include <readline/readline.h>

typedef struct e_node ASTnode;

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
	//t_token	*current_token;
    t_env   *envlist;
    int stdin;
    int stdout;
    char    **environ;
    //?? error de flux ;
    struct termios  oldt;
	ASTnode	*ast;
    int exit_status;//退出值
    bool heredoc_sigint;
    bool signint_child;
}t_shell;


/*******************main.c********************/
void    init_shell(t_shell *shell_program, char **env);
void reset_terminal(void);
/******************init_env.c*******************/
void    init_envlist(t_shell *shell_program);
char    *get_key_env(char *str);
char    *get_value_env(char *str);
void    update_envlist(t_shell *shell_program, char *key, char *value);

/******************env_util.c*******************/
t_env    *envlist_new(char *key, char *value);
void   ft_envlist_addback(t_env **envlist, t_env *new);

/******************execute_buildin.c*******************/
void excute_builtin(t_shell *shell_program, char **args);
int is_builtin(char *cmd);

/******************dispatch_commande.c*******************/
void	free_array(char **arr);
char	*find_path(char *cmd, char **envp);
void	error_commande(char *msg, int status);
void	execute(char *argv, char **envp);
void dispatch_simple_command(t_shell *shell_program, ASTnode *ast);
void dispatch_pipeline(t_shell *shell_program, ASTnode *ast);
void dispatch_command(t_shell *shell_program, ASTnode *ast);

/******************cd.c*******************/
void ft_cd(t_shell *shell_program, char **args);

/*****************echo.c***************************/
void ft_echo(t_shell *shell_program, char **args);
int ft_strcmp(const char *s1, const char *s2);

/*********************env.c************************/
void ft_env(t_shell *shell_program, t_env *env);

/********************exit.c*************************/
int ft_exit(t_shell *shell_progran,char **args);

/************************export.c************************/
t_env *ft_export(t_env *env, const char *key, const char *value);

/*********************pwd.c**********************************/
void ft_pwd(t_shell *shell_program);

/************************unset.c******************************/
t_env *ft_unset(t_env *env, const char *key);



/*****************************util.c***************************/
void error_message(t_shell *shell_program, const char *msg, int exit_s);

/***************************free.c*****************************/
void free_token(t_token *token);
void free_ast(ASTnode *node);
void free_envlist(t_env *env);
void	free_all(t_shell *shell_program);
void    exit_minishell(t_shell shell_program);

/*********************************herdoc.c*********************/
void sigint_handler(int sig);
void start_heredoc(const char *delimiter);

void print_ast(ASTnode *node, int depth);

void	ft_init_signals(t_shell *shell_program);
# endif