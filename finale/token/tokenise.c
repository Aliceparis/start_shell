#include "minishell.h"

void	ft_token(char *line, t_shell *shell_program)
{
	char	*value;

	value = NULL;
	if (!is_quote_close(line))
	{
		error_message(shell_program,
			"Error: unmatched quote found in input.", 1);
		free_token(&(shell_program->token_list));
		return ;
	}
	while (*line)
	{
		skip_space(&line);
		if (!(*line))
		{
			reset_terminal(shell_program);
			return ;
		}
		if (is_token_operator(line))
			value = get_token_operator(&line);
		else
			value = get_next_token(&line);
		if (value)
		{
			update_token_value(get_type_of_token(value),
				value, &shell_program->token_list);
			value = NULL;
		}
		else
		{
			free_token(&shell_program->token_list);
			return ;
		}
	}
}
