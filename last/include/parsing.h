#ifndef PARSING_H
# define PARSING_H
# include <stdbool.h>

/*redict_type append + heredoc ???? */
typedef enum e_node_type
{
	CMD,
	PIPE,
	IN,
	OUT,
	APPEND,
	HEREDOC,
}node_type;



typedef struct e_node
{
	node_type	type;
	char	**args;
	char	*operator;
	char	*delimiter;
	struct e_node *left;
	struct e_node *right;
}ASTnode;

/*******************parse.c*******************************/
ASTnode	*ft_parse(t_token **token, t_shell *shell_program);
ASTnode	*simple_commande(t_token **token, t_shell *shell_program);
bool is_redirect(int token_type);
int get_redirect_type(int token_type);
/*********************expansion.c**************************/
void	ft_expand_ast(ASTnode *node, t_shell *shell_program);
void	expande_commande(ASTnode *node, t_shell *shell_program);
char	*expand_word(char *str, t_shell *shell_program);
char	*content_in_single_quote(char *str, int *i);
char	*content_in_double_quote(char *str, int *i, t_shell *shell_program);
char	*content_with_variable(char *str, int *i, t_shell *shell_program);
char	*content_simple(char *str, int *i);
char	*joint_and_free(char *s1, char *s2);

void	print_ast_debug(ASTnode *node, int depth);

#endif