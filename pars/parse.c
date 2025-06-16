/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yujin <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 15:36:38 by yujin             #+#    #+#             */
/*   Updated: 2025/06/14 15:36:39 by yujin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_initial_token(t_shell *shell_program)
{
	shell_program->current_token = shell_program->token_list;
	if (!shell_program->current_token)
		return (0);
	if (shell_program->current_token->type == IS_PIPE)
	{
		ft_putstr_fd("minishell: syntax error: unexpected token `|'\n", 2);
		free_token(&(shell_program->token_list));
		return (0);
	}
	return (1);
}

int	is_syntax_error_after_pipe(t_shell *shell_program)
{
	if (!shell_program->current_token)
	{
		ft_putstr_fd("Syntax error:unexpected end command after pipe\n", 2);
		return (1);
	}
	if (shell_program->current_token
		&& shell_program->current_token->type == IS_PIPE)
	{
		ft_putstr_fd("Syntax error:unexpected end command after pipe\n", 2);
		return (1);
	}
	return (0);
}

t_ASTnode	*create_pipe_node(t_ASTnode *left, t_shell *shell_program)
{
	t_ASTnode	*node;

	node = malloc(sizeof(t_ASTnode));
	if (!node)
	{
		perror("node malloc fail\n");
		return (NULL);
	}
	ft_bzero(node, sizeof(t_ASTnode));
	node->left = left;
	shell_program->current_token = shell_program->current_token->next;
	if (is_syntax_error_after_pipe(shell_program))
	{
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
	return (node);
}

t_ASTnode	*ft_parse(t_shell *shell_program)
{
	t_ASTnode	*left;
	t_ASTnode	*node;

	if (!check_initial_token(shell_program))
		return (NULL);
	left = simple_commande(&(shell_program->current_token));
	if (!left)
		return (NULL);
	while (shell_program->current_token
		&& shell_program->current_token->type == IS_PIPE)
	{
		node = create_pipe_node(left, shell_program);
		if (!node)
			return (NULL);
		left = node;
	}
	return (left);
}
