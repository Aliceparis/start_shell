/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yujin <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 15:36:50 by yujin             #+#    #+#             */
/*   Updated: 2025/06/14 15:36:51 by yujin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	start_execution(t_shell *shell_program)
{
	init_tree(shell_program, shell_program->ast);
	ft_execute_node(shell_program, shell_program->ast);
}

void	init_tree(t_shell *shell_program, t_ASTnode *node)
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
