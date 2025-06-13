#include "minishell.h"

char	*content_with_variable(char *str, int *i, t_shell *shell_program)
{
	int		start;
	char	*resultat;
	char	*var_name;
	int		var_len;
	char	*tmp;

	if (str[*i + 1] == '?')
	{
		*i += 2;
		return (clean_old_content(ft_itoa(shell_program->exit_status), false));
	}
	start = *i + 1;
	resultat = clean_old_content(ft_strdup(""), false);
	var_len = 0;
	while (ft_isalnum(str[start + var_len]) || str[start + var_len] == '_')
		var_len++;
	if (var_len == 0)
	{
		*i += 1;
		return (ft_strdup("$"));
	}
	var_name = ft_substr(str, start, var_len);
	tmp = getenv_value(shell_program, var_name);
	free(var_name);
	if (tmp)
		resultat = clean_old_content(ft_strjoin(resultat, tmp), false);
	*i = start + var_len;
	return (resultat);
}

char	*content_simple(char *str, int *i)
{
	int		j;
	char	*resultat;

	j = *i;
	while (str[*i] && str[*i] != '\'' && str[*i] != '"' && str[*i] != '$')
		(*i)++;
	resultat = clean_old_content(ft_substr(str, j, *i - j), false);
	return (resultat);
}
