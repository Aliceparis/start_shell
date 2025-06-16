/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yujin <yujin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 13:33:35 by yujin             #+#    #+#             */
/*   Updated: 2025/06/14 15:08:25 by yujin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_token(t_token **list)
{
	t_token	*tmp;
	t_token	*next;

	tmp = *list;
	while (tmp)
	{
		free(tmp->value);
		next = tmp->next;
		free(tmp);
		tmp = next;
	}
	*list = NULL;
}

void	free_redirection(t_redict *node)
{
	t_redict	*current;
	t_redict	*next;

	current = node;
	while (current)
	{
		next = current->next;
		if (current->delimiter)
			free(current->delimiter);
		free(current);
		current = next;
	}
}

void	free_ast(t_ASTnode *node)
{
	int	i;

	if (!node)
		return ;
	if (node->left)
		free_ast(node->left);
	if (node->right)
		free_ast(node->right);
	if (node->args)
	{
		i = 0;
		while (node->args[i])
		{
			free(node->args[i]);
			i++;
		}
		free(node->args);
	}
	if (node->redirection)
		free_redirection(node->redirection);
	free(node);
}

void	free_ast_token(t_shell *shell_program)
{
	free_ast(shell_program->ast);
	shell_program->ast = NULL;
	free_token(&(shell_program->token_list));
}
