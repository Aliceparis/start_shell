/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yujin <yujin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 15:54:33 by yujin             #+#    #+#             */
/*   Updated: 2025/06/14 16:52:06 by yujin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

# ifndef PATH_MAX
#  define PATH_MAX 1024
# endif

extern volatile sig_atomic_t	g_heredoc_sigint;

typedef struct e_node			t_ast_node;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

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
	t_ASTnode		*ast;
	int				exit_status;
	bool			heredoc_sigint;
}	t_shell;

/**********************************main.c*****************/
void	init_shell(t_shell *shell_program, char **env);
void	reset_terminal(t_shell *shell_program);
/**********************************exec.c && exec2.c*************/
void	safe_close(int *fd);
void	ft_execute_node(t_shell *shell_program, t_ASTnode *node);
int		execute_simple_command(t_shell *shell_program, t_ASTnode *node);
int		execute_redirection(t_shell *shell_program, t_ASTnode *node);
int		execute_builtin_in_shell(t_shell *shell_program, t_ASTnode *cmd_node);
void	execute_externe_command(t_shell *shell_program, t_ASTnode *cmd_node);
/**********************************execute_buildin.c**************/
void	handle_export_key_value(t_shell *shell_program,
			char *key_value, char *equal_sign);
int		handle_export_arg(t_shell *shell_program, char *arg);
int		is_valid_env_var_name(const char *name);
int		excute_builtin(t_shell *shell_program, char **args);
/**********************************execute_child.c*****************/
void	handle_parent_process_wait(t_shell *shell_program, pid_t pid);
void	handle_child_process_execution(t_shell *shell_program,
			t_ASTnode *cmd_node, char *path);
/**********************************execute_simple_buildin.c*******************/
int		is_builtin(char *cmd);
int		execute_builtin_simple(t_shell *shell_program, char **args);
/**********************************gestion_fds.c***************************/
int		close_and_restore(int original_fd, int std_fd);
int		check_redirection(t_ASTnode *node);
void	free_array(char **arr);
char	**get_path(char **envp);
char	*find_path(char *cmd, char **envp);
/**********************************pipeline.c && pipeline2.c**************/
int		create_pipe(t_shell *shell_program, int *fd);
void	wait_and_return_status(t_shell *shell_program, pid_t pid1, pid_t pid2);
void	child_execute_pipe_left(t_shell *shell_program,
			t_ASTnode *left, int *fd);
void	child_execute_pipe_right(t_shell *shell_program,
			t_ASTnode *right, int *fd);
int		execute_pipeline(t_shell *shell_program, t_ASTnode *node);
/**********************************redirections.c********************/
int		handle_output_redirection(t_shell *shell_program,
			t_redict *redirection);
int		handle_append_redirection(t_shell *shell_program,
			t_redict *redirection);
int		handle_input_redirection(t_shell *shell_program, t_redict *redirection);
int		handle_heredoc_redirection(t_shell *shell_program, t_redict *redir);
int		handle_redirection(t_shell *shell_program, t_ASTnode *node);
/**********************************heredoc2.c**********************/
void	handle_heredoc_interrupt(t_shell *shell_program, int write_fd);
void	heredoc_read_loop(const char *delimiter,
			int write_fd, t_shell *shell_program);
void	execute_heredoc_child(const char *delimiter,
			int write_fd, t_shell *shell_program);
void	monitor_heredoc_child(pid_t pid, t_shell *shell_program, int read_fd);
/**********************************heredoc2.c*****************/
int		start_heredoc(const char *delimiter, t_shell *shell_program);
/**********************************cd_end.c****************************/
void	init_old_pwd(char *old_pwd_static, size_t size);
void	update_old_pwd_after_cd(char *old_pwd_static, size_t size);
/**********************************cd.c**************************/
int		ft_cd(t_shell *shell_program, char **args);
/**********************************echo.c***********************/
int		ft_echo(t_shell *shell_program, char **args);
/**********************************env.c*********************/
char	*get_key_env(char *str);
int		ft_env(t_shell *shell_program, t_env *env);
t_env	*envlist_new(char *key, char *value);
void	ft_envlist_addback(t_env **envlist, t_env *new);
/**********************************exit.c****************/
int		check_too_many_args(char **args, t_shell *shell_program);
int		check_if_number(char *arg, t_shell *shell_program);
int		ft_exit(t_shell *shell_program, char **args);
/**********************************export.c*****************************/
t_env	*ft_export(t_env *env, const char *key, const char *value);
/**********************************init_env.c***********************/
void	init_envlist(t_shell *shell_program);
char	*get_value_env(char *str);
void	update_envlist(t_shell *shell_program, char *key, char *value);
void	*clean_old_content(void *ptr, bool clean);
/****************************pwd.c*********************/
int		ft_pwd(t_shell *shell_program);
/****************************unset.c**********************/
t_env	*ft_unset(t_env *env, const char *key);
/****************************free.c***************************/
void	free_token(t_token **list);
void	free_redirection(t_redict *node);
void	free_ast(t_ASTnode *node);
void	free_ast_token(t_shell *shell_program);
void	free_envlist(t_env *env);
void	free_all(t_shell *shell_program);
/*****************************signal.c********************/
void	handle_parent_sigint(int sig);
void	setup_signe_parent(void);
void	setup_parent_for_child_execution(void);
void	reset_signals_for_child(void);
/****************************signal_heredoc.c********************/
void	handle_heredoc_sigint_child(int sig);
void	setup_heredoc_child(void);
/********************utils.c************************************/
void	error_message(t_shell *shell_program, const char *msg, int exit_s);
char	**env_list_to_array(t_env *env);
int		ft_strcmp(const char *s1, const char *s2);

#endif
