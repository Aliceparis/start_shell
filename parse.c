#include "minishell.h"

/*1. simple commande: token type is_word 类别，直接malloc到node的cmd type里面
  2. token type是pipe时，token跳过，然后左边的分支就是之前node的内容，右边分支就是next token，然后把右边的也变成cmd类型的node，然后更新总的node的左边就是现在的这个node
  3. token type是redirection的时候，把operator update，然后左边是之前那个node，右边是file类型，然后把左边的整体update到现在的这个node
  4. 当token类型什么也不是的时候，结束运行*/

ASTnode	*ft_parse(t_token **token)
{
	ASTnode *left;
	ASTnode	*node;
	
	left = simple_commande(token);
	while (*token)
	{
		if ((*token)->type == IS_PIPE)
		{
			node = malloc(sizeof(ASTnode));
			if (!node)
		////fonction error and free (error: malloc echoue)
			*token = (*token)->next;
			node->left = left;
			node->type = PIPE;
			node->right = ft_parse(token);
			left = node;
		}
		else if ((*token)->type == REDICT_OUT || (*token)->type == REDICT_IN)
		{
			node = malloc(sizeof(ASTnode));
			if (!node)
		////fonction error and free (error: malloc echoue)
			node->operator = (*token)->value;
			*token = (*token)->next;
			if (!(*token) && || (*token)->type != IS_WORD)
				////fonction error and free (error: expected filename or deliminiter)
			node->file = ft_strdup((*token)->value);
			*token = (*token)->next;
			node->type = REDIRECTION;
			node->left = left;
			node->right = ft_parse(token);
			left = node;
		}
		else
			break ;
	}
	return (left);
}

ASTnode	*simple_commande(t_token **token)
{
	t_token	*tmp;
	int	count_args;
	ASTnode	*node;
	char	**args;
	int	i;

	tmp = *token;
	count_args = 0;
	i = 0;
	while (tmp && tmp->type == IS_WORD)
	{
		count_args++;
		tmp = tmp->next;
	}
	args = malloc(sizeof(char *) * (count_args + 1));
	if (!args)
		////fonction error and free (error: malloc echoue)
	while (*token && (*token)->type == IS_WORD)
	{
		args[i++] = ft_strdup((*token)->value);
		*token = (*token)->next;
	}
	args[i] = NULL;
	node = malloc(sizeof(ASTnode));
	if (!node)
	////fonction error and free (error: malloc echoue)
	node->type = CMD;
	node->args = args;
	node->left = node->right = NULL;
	return (node);
}
