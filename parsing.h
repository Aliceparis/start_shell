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

#endif