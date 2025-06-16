/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yujin <yujin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 13:31:40 by yujin             #+#    #+#             */
/*   Updated: 2025/06/14 15:07:00 by yujin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Fonction 1 : alloue et prépare le tableau expanded_args
char	**allocate_expanded_args(char **args)
{
	int		count_arg;
	char	**expanded_args;

	count_arg = 0;
	if (!args)
		return (NULL);
	while (args[count_arg])
		count_arg++;
	expanded_args = clean_old_content(
			malloc(sizeof(char *) * (count_arg + 1)), false);
	if (!expanded_args)
	{
		printf("expansion args malloc fail\n");
		return (NULL);
	}
	return (expanded_args);
}

// Fonction 2 : effectue l'expansion et remplit expanded_args
int	fill_expanded_args(char **args, char **expanded_args,
			t_shell *shell_program)
{
	int		i;
	char	*expanded;

	i = 0;
	if (!args || !expanded_args)
		return (0);
	while (args[i])
	{
		expanded = expand_word(args[i], shell_program);
		if (!expanded)
		{
			printf("Error: expansion failed for argument \n");
			return (0);
		}
		expanded_args[i] = expanded;
		i++;
	}
	expanded_args[i] = NULL;
	return (1);
}

void	expande_commande(t_ASTnode *node, t_shell *shell_program)
{
	char	**expanded_args;

	if (!node->args)
		return ;
	expanded_args = allocate_expanded_args(node->args);
	if (!expanded_args)
		return ;
	if (!fill_expanded_args(node->args, expanded_args, shell_program))
		return ;
	node->expanded_args = expanded_args;
}
