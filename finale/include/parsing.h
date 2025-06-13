#ifndef PARSING_H
# define PARSING_H
# include <stdbool.h>

/*redict_type append + heredoc ???? */
typedef enum e_node_type
{
	CMD,
	PIPE,
}	node_type;

typedef enum e_redict_type
{
	IN,
	OUT,
	APPEND,
	HEREDOC,
}	redict_type;

typedef struct e_redict
{
	redict_type		type;
	char			*delimiter;
	struct e_redict	*next;
}	s_redict;

typedef struct e_node
{
	node_type		type;
	s_redict		*redirection;
	char			**args;
	char			**expanded_args;
	struct e_node	*left;
	struct e_node	*right;
}	ASTnode;

/*******************parse.c*******************************/
ASTnode		*ft_parse(t_shell *shell_program);
void		init_tree(t_shell *shell_program, ASTnode *node);
void		start_execution(t_shell *shell_program);
/*******************parse_redirection.c*******************************/
int			is_direction(token_type type);
int			get_redirect_type(int token_type);
int	get_delimiter(t_token	**token, s_redict *new);
void	*parse_single_redirection(t_token **token);
s_redict    *parse_redirections(t_token **token);

/*******************parse_simple_commande.c*******************************/
ASTnode *create_commande_node(void);
int append_redirections_to_cmd(ASTnode *cmd_node, s_redict *new);
ASTnode *simple_commande(t_token **token);
char    **get_parse_commande_args(t_token **token);

/*********************expansion.c**************************/
void		expande_commande(ASTnode *node, t_shell *shell_program);
char		*expand_word(char *str, t_shell *shell_program);
char		*content_in_single_quote(char *str, int *i);
char		*content_in_double_quote(char *str, int *i, t_shell *shell_program);
char		*content_with_variable(char *str, int *i, t_shell *shell_program);
char		*content_simple(char *str, int *i);
char		*getenv_value(t_shell *shell_program, const char *var_name);
void		init_tree(t_shell *shell_program, ASTnode *node);
void		start_execution(t_shell *shell_program);
void		ft_execute_node(t_shell *shell_program, ASTnode *node);
void		dispatch_heredoc(t_shell *shell_program, ASTnode *ast);
bool		should_exit_on_error(int exit_status);

#endif