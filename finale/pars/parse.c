/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmen <jmen@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 16:28:42 by yujin             #+#    #+#             */
/*   Updated: 2025/06/13 12:59:22 by jmen             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	start_execution(t_shell *shell_program)
{
	init_tree(shell_program, shell_program->ast);
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

ASTnode	*ft_parse(t_shell *shell_program)
{
	ASTnode	*left;
	ASTnode	*node;

	shell_program->current_token = shell_program->token_list;
	if (!shell_program->current_token)
		return (NULL);
	if (shell_program->current_token->type == IS_PIPE)
	{
		ft_putstr_fd("minishell: syntax error: unexpected token `|'\n", 2);
		free_token(&(shell_program->token_list));
		return (NULL);
	}
	left = simple_commande(&(shell_program->current_token));
	if (!left)
		return (NULL);
	while (shell_program->current_token
		&& shell_program->current_token->type == IS_PIPE)
	{
		node = malloc(sizeof(ASTnode));
		if (!node)
			return (perror("node malloc fail\n"), NULL);
		ft_bzero(node, sizeof(ASTnode));
		node->left = left;
		shell_program->current_token = shell_program->current_token->next;
		if (shell_program->current_token
			&& shell_program->current_token->type == IS_PIPE)
		{
			ft_putstr_fd("Syntax error:unexpected end command after pipe\n", 2);
			free_ast(node);
			return (NULL);
		}
		if (!shell_program->current_token)
		{
			ft_putstr_fd("Syntax error:unexpected end command after pipe\n", 2);
			free_ast(node);
			return (NULL);
		}
		node->type = PIPE;
		node->right = simple_commande(&(shell_program->current_token));
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
