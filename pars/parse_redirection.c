/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yujin <yujin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 16:28:48 by yujin             #+#    #+#             */
/*   Updated: 2025/06/14 15:04:30 by yujin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*parse_single_redirection(t_token **token)
{
	t_redict	*new;

	if (!(*token) || !is_direction((*token)->type))
		return (NULL);
	new = malloc(sizeof(t_redict));
	if (!new)
	{
		perror("malloc parse redirection failed\n");
		return (NULL);
	}
	ft_bzero(new, sizeof(t_redict));
	new->type = get_redirect_type((*token)->type);
	new->next = NULL;
	*token = (*token)->next;
	if (!get_delimiter(token, new))
	{
		free_redirection(new);
		return (NULL);
	}
	return (new);
}

int	get_delimiter(t_token	**token, t_redict *new)
{
	if (*token && (*token)->type == IS_WORD)
	{
		new->delimiter = ft_strdup((*token)->value);
		if (!new->delimiter)
		{
			perror("strdup redirection delimiter failed");
			free(new);
			return (0);
		}
		*token = (*token)->next;
		return (1);
	}
	else
	{
		perror("Syntax error near unexpected token \n");
		return (0);
	}
}

t_redict	*parse_redirections(t_token **token)
{
	t_redict	*new;
	t_redict	*head;

	head = NULL;
	while (*token && is_direction((*token)->type))
	{
		new = parse_single_redirection(token);
		if (!new)
		{
			free_redirection(new);
			return (NULL);
		}
		if (!head)
			head = new;
		else
		{
			while (head && head->next)
				head = head->next;
			head->next = new;
		}
	}
	return (head);
}

int	is_direction(t_token_type type)
{
	if (type == REDICT_APPEND || type == REDICT_IN
		|| type == REDICT_OUT || type == IS_HEREDOC)
		return (1);
	return (0);
}

int	get_redirect_type(int t_token_type)
{
	if (t_token_type == IS_HEREDOC)
		return (HEREDOC);
	if (t_token_type == REDICT_IN)
		return (IN);
	if (t_token_type == REDICT_OUT)
		return (OUT);
	if (t_token_type == REDICT_APPEND)
		return (APPEND);
	return (-1);
}
