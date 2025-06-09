#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include "tokenising.h"
# include "parsing.h"

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <termios.h>
# include <signal.h>
# include <stdbool.h>
# include <sys/wait.h>
# include <errno.h>

# include <readline/history.h>
# include <readline/readline.h>

# ifndef BUFF
#  define BUFF 1024
# endif

typedef struct e_node ASTnode;
extern  volatile sig_atomic_t g_signal_status;

typedef struct s_env
{
	char	*key;
	char	*value;
	struct s_env	*next;
}	t_env;

typedef enum 
{
    SIGNAL_NORMAL = 0,
    SIGNAL_CHILD = 1,
    SIGNAL_HEREDOC = 2,
    SIGNAL_INTERRUP = 130,
}t_signal_status;

typedef struct s_shell
{
	char			*line;
	t_token			*token_list;
	t_token			*current_token;
	t_env			*envlist;
	int				stdin;
	int				stdout;
	char			**environ;
	struct termios	oldt;
	ASTnode			*ast;
	int				exit_status;
	bool			heredoc_sigint;
}	t_shell;

int		check_if_number(char *arg, t_shell *shell_program);
void	handle_parent_sigint(int sig);
void	setup_parent_signals(void);
void	reset_signals_for_child(void);

/*******************main.c********************/
void	init_shell(t_shell *shell_program, char **env);
void	reset_terminal(t_shell *shell_program);
/*void	reset_signals_in_child(void);
void	handle_signal(int sig);
void	ft_init_signals(t_shell *shell_program);*/

/******************init_env.c*******************/
void	init_envlist(t_shell *shell_program);
char	*get_key_env(char *str);
char	*get_value_env(char *str);
void	update_envlist(t_shell *shell_program, char *key, char *value);
void	*clean_old_content(void *ptr, bool clean);

/******************env_util.c*******************/
t_env	*envlist_new(char *key, char *value);
void	ft_envlist_addback(t_env **envlist, t_env *new);

/******************execute_buildin.c*******************/
int	excute_builtin(t_shell *shell_program, char **args);
int		is_builtin(char *cmd);
int	execute_builtin_simple(t_shell *shell_program, char **args);
int	handle_redirection(t_shell *shell_program, ASTnode *node);
int execute_builtin_in_shell(t_shell *shell_program, ASTnode *cmd_node);
int  handle_export_arg(t_shell *shell_program, char *arg);


/******************cd.c*******************/
int	ft_cd(t_shell *shell_program, char **args);

/*****************echo.c***************************/
int	ft_echo(t_shell *shell_program, char **args);
int		ft_strcmp(const char *s1, const char *s2);

/*********************env.c************************/
int	ft_env(t_shell *shell_program, t_env *env);

/********************exit.c*************************/
int		ft_exit(t_shell *shell_progran,char **args);

/************************export.c************************/
t_env	*ft_export(t_env *env, const char *key, const char *value);

/*********************pwd.c**********************************/
int	ft_pwd(t_shell *shell_program);

/************************unset.c******************************/
t_env	*ft_unset(t_env *env, const char *key);

/*****************************util.c***************************/
void	error_message(t_shell *shell_program, const char *msg, int exit_s);

/***************************free.c*****************************/
void	free_token(t_token **list);
void	free_ast(ASTnode *node);
void	free_envlist(t_env *env);
void	free_all(t_shell *shell_program);
void	exit_minishell(t_shell *shell_program);
void	free_cmd_node(ASTnode *node);
//void free_ast(t_shell *shell_program);
void	free_redirection(s_redict *node);

/*********************************herdoc.c*********************/
//void sigint_handler(int sig);
int		start_heredoc(const char *delimiter, t_shell *shell_program);
void	heredoc_loop(const char *delimiter, int write_fd, t_shell *shell_program);
//void print_ast(ASTnode *node, int depth);

/*********************************gestion_fds.c*********************/
void	free_array(char **arr);
char	*find_path(char *cmd, char **envp);
void	close_and_restore(int original_fd, int std_fd);
void	close_pipe_fds(int fd[2]);
int		check_redirection(ASTnode *node);

/*********************************exec.c*********************/
void	ft_execute_node(t_shell *shell_program, ASTnode *node);
int execute_simple_command(t_shell *shell_program, ASTnode *node);
int execute_redirection(t_shell *shell_program, ASTnode *node);
int execute_builtin_in_shell(t_shell *shell_program, ASTnode *cmd_node);
void	execute_externe_command(t_shell *shell_program, ASTnode *cmd_node);

/*********************************pipeline.c*********************/
int execute_pipeline(t_shell *shell_program, ASTnode *node);


void set_heredoc_sig(void);
void	handle_parent_heredoc(int sig);
void restore_parent_signals(void);
void setup_execution_signals(void);
void restore_shell_signals(void);

int		is_valid_env_var_name(const char *name);
char	**env_list_to_array(t_env *env);
#endif