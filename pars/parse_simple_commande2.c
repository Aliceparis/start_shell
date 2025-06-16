/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_simple_commande2.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yujin <yujin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 15:45:03 by yujin             #+#    #+#             */
/*   Updated: 2025/06/14 16:03:11 by yujin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_pipe_error(t_token **token, t_ASTnode *cmd_node)
{
	if (*token && (*token)->type == IS_PIPE)
	{
		ft_putstr_fd("minishell: syntax error: unexpected token `|'\n", 2);
		free(cmd_node);
		return (1);
	}
	return (0);
}

int	pars_handle_redirection(t_token **token, t_ASTnode *cmd_node)
{
	t_redict	*redirections;

	redirections = parse_redirections(token);
	if (!redirections)
		return (-1);
	if (!append_redirections_to_cmd(cmd_node, redirections))
		return (-1);
	return (0);
}

int	handle_command_args(t_token **token, t_ASTnode *cmd_node)
{
	char	**pars_args;

	if (cmd_node->args)
	{
		perror("syntax error");
		return (-1);
	}
	pars_args = get_parse_commande_args(token);
	if (!pars_args)
		return (-1);
	cmd_node->args = pars_args;
	return (0);
}

int	parse_tokens(t_token **token, t_ASTnode *cmd_node)
{
	while (*token && (*token)->type != IS_PIPE)
	{
		if (is_direction((*token)->type))
		{
			if (pars_handle_redirection(token, cmd_node) == -1)
				return (-1);
		}
		else if ((*token)->type == IS_WORD)
		{
			if (handle_command_args(token, cmd_node) == -1)
				return (-1);
		}
		else
		{
			perror("syntax error near unexpected token");
			return (-1);
		}
	}
	return (0);
}

t_ASTnode	*simple_commande(t_token **token)
{
	t_ASTnode	*cmd_node;

	cmd_node = create_commande_node();
	if (!cmd_node)
		return (NULL);
	if (check_pipe_error(token, cmd_node))
		return (NULL);
	if (parse_tokens(token, cmd_node) == -1)
	{
		free_ast(cmd_node);
		return (NULL);
	}
	return (cmd_node);
}
