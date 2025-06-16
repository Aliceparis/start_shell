/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yujin <yujin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 13:34:44 by yujin             #+#    #+#             */
/*   Updated: 2025/06/14 14:47:06 by yujin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*ft_token:
	1. skip espaces
	2. check the double quote and get the value between the quote
	3. check the token type, and get the token type seperator
	4. get others information, is the word token type
	5. update the type and the value ine the structure*/
int	ft_isspace(char c)
{
	if ((c >= 9 && c <= 13) || c == ' ')
		return (1);
	return (0);
}

void	skip_space(char **input)
{
	while (**input && ft_isspace(**input))
		(*input)++;
}

int	is_quote(char c)
{
	if (c == '"' || c == '\'')
		return (1);
	return (0);
}

int	is_token_operator(char *input)
{
	if (!ft_strncmp(input, ">>", 2) || !ft_strncmp(input, "<<", 2)
		|| *input == '>' || *input == '<' || *input == '|')
		return (1);
	return (0);
}

t_token_type	get_type_of_token(char *str)
{
	if (!ft_strncmp(str, ">>", 2))
		return (REDICT_APPEND);
	if (!ft_strncmp(str, "<<", 2))
		return (IS_HEREDOC);
	if (*str == '|')
		return (IS_PIPE);
	if (*str == '>')
		return (REDICT_OUT);
	if (*str == '<')
		return (REDICT_IN);
	return (IS_WORD);
}
