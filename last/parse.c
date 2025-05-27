#include "minishell.h"

/*1. simple commande: token type is_word 类别，直接malloc到node的cmd type里面
  2. token type是pipe时，token跳过，然后左边的分支就是之前node的内容，右边分支就是next token，然后把右边的也变成cmd类型的node，然后更新总的node的左边就是现在的这个node
  3. token type是redirection的时候，把operator update，然后左边是之前那个node，右边是file类型，然后把左边的整体update到现在的这个node
  4. 当token类型什么也不是的时候，结束运行*/

  /*？？？？token type rediction append + redirection heredoc 有待处理*/
ASTnode *ft_parse(t_token **token, t_shell *shell_program) 
{
    if (!token || !(*token))  // 保护
        return (NULL);
    // 解析左侧命令
    ASTnode *left = simple_commande(token, shell_program);
    if (!left) 
	{
        // 如果左侧无命令且当前是 `|`，报错（如 `| cmd` 或 `|`）
        if (*token && (*token)->type == IS_PIPE) 
		{
            error_message(shell_program, "Syntax error: Missing command before '|'\n", 1);
            return NULL;
        }
    }

    while (*token) 
	{
        if ((*token)->type == IS_PIPE) 
		{
            // 检查是否缺少右侧命令（如 `cmd |` 或 `cmd1 | | cmd2`）
            if (!(*token)->next || (*token)->next->type == IS_PIPE) 
			{
                error_message(shell_program, "Syntax error: Missing command after '|'\n", 1);
				//free_all(shell_program);
				reset_terminal(shell_program);
            }

            // 创建管道节点
            ASTnode *node = clean_old_content(malloc(sizeof(ASTnode)), false);
            if (!node) 
			{
                error_message(shell_program, "Error: Malloc Astnode failed.\n", 1);
                free_all(shell_program);
            }

            *token = (*token)->next;  // 跳过 `|`
            node->left = left;
            node->type = PIPE;
            node->right = ft_parse(token, shell_program);  // 递归解析右侧
            node->args = NULL;
            node->operator = NULL;
            node->delimiter = NULL;
            left = node;  // 更新 left 为管道节点
        }
        else if (is_redirect((*token)->type)) 
		{
            // 创建重定向节点
            ASTnode *node = clean_old_content(malloc(sizeof(ASTnode)), false);
            if (!node) 
			{
                error_message(shell_program, "Error: Malloc Astnode failed.\n", 1);
                free_all(shell_program);
            }

            node->operator = clean_old_content(ft_strdup((*token)->value), false);
            node->type = get_redirect_type((*token)->type);
            *token = (*token)->next;  // 跳过 `>`, `<` 等

            // 检查是否缺少文件名（如 `echo >`）
            if (!(*token) || (*token)->type != IS_WORD) 
			{
                error_message(shell_program, "Syntax error: Missing filename after redirect\n", 1);
				free_all(shell_program);
                return NULL;
            }

            node->delimiter = clean_old_content(ft_strdup((*token)->value), false);
            *token = (*token)->next;  // 跳过文件名

            node->left = left;
            node->right = ft_parse(token, shell_program);
            node->args = NULL;
            left = node;  // 更新 left 为重
		}
	}
	return(left);
}

bool is_redirect(int token_type) 
{
    return (token_type == REDICT_OUT || token_type == IS_HEREDOC ||
            token_type == REDICT_IN || token_type == REDICT_APPEND);
}

int get_redirect_type(int token_type) 
{
    if (token_type == IS_HEREDOC) return HEREDOC;
    if (token_type == REDICT_IN) return IN;
    if (token_type == REDICT_OUT) return OUT;
    if (token_type == REDICT_APPEND) return APPEND;
    return -1;  // 错误类型
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
	args = clean_old_content(malloc(sizeof(char *) * (count_args + 1)), false);
	if (!args)
	{
		error_message(shell_program, "Error: Malloc args failed.\n", 1);
		free_all(shell_program);
	}
	while (*token && (*token)->type == IS_WORD)
	{
		args[i++] = clean_old_content(ft_strdup((*token)->value), false);
		*token = (*token)->next;
	}
	args[i] = NULL;
	node =clean_old_content(malloc(sizeof(ASTnode)), false);
	if (!node)
	{
		error_message(shell_program, "Error: Malloc Astnode failed.\n", 1);
		free_all(shell_program);
	}
	node->type = CMD;
	node->args = args;
	node->operator = NULL;
	node->left = node->right =  NULL;
	return (node);
}
/*
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
}*/
