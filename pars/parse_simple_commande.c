/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_simple_commande.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yujin <yujin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 16:28:56 by yujin             #+#    #+#             */
/*   Updated: 2025/06/14 15:49:19 by yujin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ASTnode	*create_commande_node(void)
{
	t_ASTnode	*cmd_node;

	cmd_node = malloc(sizeof(t_ASTnode));
	if (!cmd_node)
		return (perror("malloc simple cmd error \n"), NULL);
	ft_bzero(cmd_node, sizeof(t_ASTnode));
	cmd_node->type = CMD;
	return (cmd_node);
}

int	append_redirections_to_cmd(t_ASTnode *cmd_node, t_redict *new)
{
	t_redict	*tmp;

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

int	count_args(t_token *token)
{
	int	count;

	count = 0;
	while (token && token->type == IS_WORD)
	{
		count++;
		token = token->next;
	}
	return (count);
}

char	**get_parse_commande_args(t_token **token)
{
	int		count_a;
	int		i;
	char	**args;

	count_a = count_args(*token);
	i = 0;
	args = malloc(sizeof(char *) * (count_a + 1));
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
