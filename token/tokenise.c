/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenise.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yujin <yujin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 16:39:58 by yujin             #+#    #+#             */
/*   Updated: 2025/06/14 16:51:44 by yujin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_quote_close(char *str)
{
	char	quote;

	quote = 0;
	while (*str)
	{
		if (*str == '"' || *str == '\'')
		{
			if (!quote)
				quote = *str;
			else if (quote == *str)
				quote = 0;
		}
		str++;
	}
	if (!quote)
		return (1);
	return (0);
}

int	check_quotes_and_error(char *line, t_shell *shell_program)
{
	if (!is_quote_close(line))
	{
		error_message(shell_program,
			"Error: unmatched quote found in input.", 1);
		free_token(&(shell_program->token_list));
		return (0);
	}
	return (1);
}

// Fonction pour traiter chaque token dans la ligne
static int	process_token(char **line, t_shell *shell_program)
{
	char	*value;

	value = NULL;
	skip_space(line);
	if (!**line)
	{
		reset_terminal(shell_program);
		return (0);
	}
	if (is_token_operator(*line))
		value = get_token_operator(line);
	else
		value = get_next_token(line);
	if (value)
	{
		update_token_value(get_type_of_token(value),
			value, &shell_program->token_list);
		return (1);
	}
	else
	{
		free_token(&shell_program->token_list);
		return (0);
	}
}

void	ft_token(char *line, t_shell *shell_program)
{
	if (!check_quotes_and_error(line, shell_program))
		return ;
	while (*line)
	{
		if (!process_token(&line, shell_program))
			return ;
	}
}
