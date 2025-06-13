/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_simple_commande.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmen <jmen@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 16:28:56 by yujin             #+#    #+#             */
/*   Updated: 2025/06/13 11:57:20 by jmen             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

ASTnode	*create_commande_node(void)
{
	ASTnode	*cmd_node;

	cmd_node = malloc(sizeof(ASTnode));
	if (!cmd_node)
		return (perror("malloc simple cmd error \n"), NULL);
	ft_bzero(cmd_node, sizeof(ASTnode));
	cmd_node->type = CMD;
	return (cmd_node);
}

int	append_redirections_to_cmd(ASTnode *cmd_node, s_redict *new)
{
	s_redict	*tmp;

	if (!cmd_node || !new)
		return (0);
	if (!cmd_node->redirection)
		cmd_node->redirection = new;
	else
	{
		tmp = cmd_node->redirection;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
	return (1);
}

ASTnode	*simple_commande(t_token **token)
{
	ASTnode		*cmd_node;
	s_redict	*redirections;
	char		**pars_args;

	cmd_node = create_commande_node();
	if (!cmd_node)
		return (NULL);
	if (*token && (*token)->type == IS_PIPE)
	{
		ft_putstr_fd("minishell: syntax error: unexpected token `|'\n", 2);
		free(cmd_node);
		return (NULL);
	}
	while (*token && (*token)->type != IS_PIPE)
	{
		if (is_direction((*token)->type))
		{
			redirections = parse_redirections(token);
			if (!redirections)
			{
				free_ast(cmd_node);
				return ( NULL);
			}
			if (!append_redirections_to_cmd(cmd_node, redirections))
			{
				free_ast(cmd_node);
				return (NULL);
			}
		}
		else if ((*token)->type == IS_WORD)
		{
			if (cmd_node->args)
			{
				perror("syntax error: arguments cannot be interspersed after redirection or existing arguments.");
				free_ast(cmd_node);
				return (NULL);
			}
			pars_args = get_parse_commande_args(token);
			if (!pars_args)
			{
				free_ast(cmd_node);
				return ( NULL);

			}
			cmd_node->args = pars_args;
		}
		else
		{
			perror("syntax error near unexpected token ");
			free_ast(cmd_node);
			return ( NULL);
		}
	}
	return (cmd_node);
}

char	**get_parse_commande_args(t_token **token)
{
	int		count_args;
	int		i;
	t_token	*tmp;
	char	**args;

	count_args = 0;
	i = 0;
	tmp = *token;
	args = NULL;
	while (tmp && tmp->type == IS_WORD)
	{
		count_args++;
		tmp = tmp->next;
	}
	args = malloc(sizeof(char *) * (count_args + 1));
	if (!args)
	{
		while (i > 0)
			free(args[--i]);
		free(args);
		return (perror("malloc args fail\n"), NULL);
	}
	while (*token && (*token)->type == IS_WORD)
	{
		args[i++] = ft_strdup((*token)->value);
		*token = (*token)->next;
	}
	args[i] = NULL;
	return (args);
}
