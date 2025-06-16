/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yujin <yujin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 15:09:42 by yujin             #+#    #+#             */
/*   Updated: 2025/06/14 16:04:46 by yujin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H
# include <stdbool.h>

typedef enum e_node_type
{
	CMD,
	PIPE,
}	t_node_type;

typedef enum e_redict_type
{
	IN,
	OUT,
	APPEND,
	HEREDOC,
}	t_redict_type;

typedef struct e_redict
{
	t_redict_type	type;
	char			*delimiter;
	struct e_redict	*next;
}	t_redict;

typedef struct e_node
{
	t_node_type		type;
	t_redict		*redirection;
	char			**args;
	char			**expanded_args;
	struct e_node	*left;
	struct e_node	*right;
}	t_ASTnode;

/*******************parse.c*******************************/
t_ASTnode	*ft_parse(t_shell *shell_program);
void		init_tree(t_shell *shell_program, t_ASTnode *node);
void		start_execution(t_shell *shell_program);
/*******************parse_redirection.c*****************/
int			is_direction(t_token_type type);
int			get_redirect_type(int t_token_type);
int			get_delimiter(t_token	**token, t_redict *new);
void		*parse_single_redirection(t_token **token);
t_redict	*parse_redirections(t_token **token);
/*******************parse_simple_commande.c*********************/
t_ASTnode	*create_commande_node(void);
int			append_redirections_to_cmd(t_ASTnode *cmd_node, t_redict *new);
t_ASTnode	*simple_commande(t_token **token);
char		**get_parse_commande_args(t_token **token);

/********************************expansion.c***************************/
char		*expand_word(char *str, t_shell *shell_program);
char		*content_in_single_quote(char *str, int *i);
char		*content_in_double_quote(char *str, int *i, t_shell *shell_program);
char		*getenv_value(t_shell *shell_program, const char *var_name);
/********************************expand_variable.c*************************/
char		*extract_var_name(char *str, int *i);
char		*content_with_variable(char *str, int *i, t_shell *shell_program);
char		*content_simple(char *str, int *i);
/********************************expand_variable.c**********************/
void		init_tree(t_shell *shell_program, t_ASTnode *node);
void		start_execution(t_shell *shell_program);
void		ft_execute_node(t_shell *shell_program, t_ASTnode *node);
/******************************expansion2.c***********************/
char		**allocate_expanded_args(char **args);
int			fill_expanded_args(char **args,
				char **expanded_args, t_shell *shell_program);
void		expande_commande(t_ASTnode *node, t_shell *shell_program);

#endif
