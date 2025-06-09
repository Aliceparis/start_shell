# include "minishell.h"

void	start_execution(t_shell *shell_program)
{
	init_tree(shell_program, shell_program->ast);
	if (shell_program->heredoc_sigint)
	{
		printf("ctrlc in start\n");
		free_all(shell_program);
		shell_program->heredoc_sigint = false;
	}
	ft_execute_node(shell_program, shell_program->ast);
}

void	init_tree(t_shell *shell_program, ASTnode *node)
{
	if (!node)
		return ;
	if (node->type == PIPE)
	{
		init_tree(shell_program, node->left);
		if (!shell_program->heredoc_sigint)
			init_tree(shell_program, node->right);
	}
	else
		expande_commande(node, shell_program);
}

ASTnode *ft_parse(t_token **token)
{
    ASTnode *left;
    ASTnode *node;

    if (!(*token) || !token)
		return (NULL);
	if ((*token)->type == IS_PIPE)
	{
        ft_putstr_fd("minishell: syntax error: unexpected token `|'\n", 2);
		return ( NULL);
	}
    left = simple_commande(token);
	if (!left)
		return (NULL);
    while(*token && (*token)->type == IS_PIPE)
	{
		node = malloc(sizeof(ASTnode));
		if (!node)
			return (perror("node malloc fail\n"), NULL);
		ft_bzero(node, sizeof(ASTnode));
		node->left = left;
		*token = (*token)->next;
        if (*token && (*token)->type == IS_PIPE)
        {
            ft_putstr_fd("Syntax error :  unexpected end of command after pipe\n", 2);
            free_ast(node);
            return (NULL);
        }
		if (!(*token))
		{
            ft_putstr_fd("Syntax error :  unexpected end of command after pipe\n", 2);
            free_ast(node);
            return (NULL);
        }
		node->type = PIPE;
        node->right = simple_commande(token);
        if (!node->right)
        {
			ft_putstr_fd("minishell: syntax error: unexpected token `|'\n", 2);
            free_ast(node);
            return (NULL);
        }
        left = node;
    }
    return (left);
}
