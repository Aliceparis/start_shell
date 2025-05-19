#include "minishell.h"

/*1. simple commande: token type is_word 类别，直接malloc到node的cmd type里面
  2. token type是pipe时，token跳过，然后左边的分支就是之前node的内容，右边分支就是next token，然后把右边的也变成cmd类型的node，然后更新总的node的左边就是现在的这个node
  3. token type是redirection的时候，把operator update，然后左边是之前那个node，右边是file类型，然后把左边的整体update到现在的这个node
  4. 当token类型什么也不是的时候，结束运行*/

  /*？？？？token type rediction append + redirection heredoc 有待处理*/
ASTnode	*ft_parse(t_token **token, t_shell *shell_program)
{
	ASTnode *left;
	ASTnode	*node;
	
	if (!token || !(*token)) //保护
		return (NULL);
	left = simple_commande(token, shell_program);
	while (*token)
	{
		if ((*token)->type == IS_PIPE)
		{
			node = malloc(sizeof(ASTnode));
			if (!node)
			{
				error_message(shell_program, "Error: Malloc Astnode failed.\n", 1);
				free_all(shell_program);
			}
			*token = (*token)->next;
			node->left = left;
			node->type = PIPE;
			node->right = ft_parse(token, shell_program);
			left = node;
		}
		else if ((*token)->type == REDICT_OUT || (*token)->type == REDICT_IN || (*token)->type == IS_HEREDOC || (*token)->type == REDICT_APPEND)
		{
			node = malloc(sizeof(ASTnode));
			if (!node)
		////fonction error and free (error: malloc echoue)
			{
				error_message(shell_program, "Error: Malloc Astnode failed.\n", 1);
				free_all(shell_program);
			}
			node->operator = (*token)->value;
			*token = (*token)->next;
			if (!(*token) && (*token)->type != IS_WORD)
			{
				error_message(shell_program, "Error: token est null.\n", 1);
				free_all(shell_program);
			}
			node->file = clean_old_content(ft_strdup((*token)->value), false);
			*token = (*token)->next;
			node->type = REDIRECTION;
			node->left = left;
			node->right = ft_parse(token, shell_program);
			left = clean_old_content(node, false);
		}
		else
			break ;
	}
	free_token(token);
	return (left);
}

ASTnode	*simple_commande(t_token **token, t_shell *shell_program)
{
	t_token	*tmp;
	int	count_args;
	ASTnode	*node;
	char	**args;
	int	i;

	tmp = *token;
	count_args = 0;
	args = NULL;
	node = NULL;
	i = 0;
	while (tmp && tmp->type == IS_WORD)
	{
		count_args++;
		tmp = tmp->next;
	}
	args = malloc(sizeof(char *) * (count_args + 1));
	if (!args)
		////fonction error and free (error: malloc echoue)
	{
		error_message(shell_program, "Error: Malloc args failed.\n", 1);
		free_all(shell_program);
	}
	while (*token && (*token)->type == IS_WORD)
	{
		args[i++] =clean_old_content(ft_strdup((*token)->value), false);
		*token = (*token)->next;
	}
	args[i] = NULL;
	node = malloc(sizeof(ASTnode));
	if (!node)
	{
		error_message(shell_program, "Error: Malloc Astnode failed.\n", 1);
		free_all(shell_program);
	}
	node->type = CMD;
	node->args = args;
	node->file = NULL;
	node->operator = NULL;
	node->left = node->right =  NULL;
	return (node);
}
void print_ast(ASTnode *node, int depth)
{
    if (!node)
        return;
    for (int i = 0; i < depth; i++)
        printf("  ");
    printf("Node type: %d\n", node->type);  // 你也可以写成字符串形式

    if (node->args)
    {
        for (int i = 0; node->args[i]; i++)
        {
            for (int j = 0; j < depth; j++)
                printf("  ");
            printf("type : %u, Arg: %s\n", node->type, node->args[i]);
        }
    }
    if (node->file)
    {
        for (int j = 0; j < depth; j++)
            printf("  ");
        printf("File: %s\n", node->file);
    }

    print_ast(node->left, depth + 1);
    print_ast(node->right, depth + 1);
}
