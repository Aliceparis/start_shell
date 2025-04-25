#ifndef PARSING_H
# define PARSING_H


/*redict_type append + heredoc ???? */
typedef enum e_node_type
{
	CMD,
	PIPE,
	REDIRECTION,
}node_type;

typedef enum	e_redict_type
{
	IN,
	OUT,
	APPEND,
	HEREDOC,
}redict_type;

typedef struct e_node
{
	node_type	type;
	char	**args;
	char	*file;
	char	*operator;
	redict_type  r_type;
	struct e_node *left;
	struct e_node *right;
}ASTnode;

/*******************parse.c*******************************/
ASTnode	*ft_parse(t_token **token);
ASTnode	*simple_commande(t_token **token);

/*********************expansion.c**************************/
void	ft_expand_ast(ASTnode *node);
void	expande_commande(ASTnode *node);
char	*expand_word(char *str);
char	*content_in_single_quote(char *str, int *i);
char	*content_in_double_quote(char *str, int *i);
char	*content_with_variable(char *str, int *i);
char	*content_simple(char *str, int *i);
char	*joint_and_free(char *s1, char *s2);

#endif