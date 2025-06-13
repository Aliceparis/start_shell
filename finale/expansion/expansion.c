#include "minishell.h"

void	expande_commande(ASTnode *node, t_shell *shell_program)
{
	char	*expanded;
	int		count_arg;
	char	**expanded_args;
	int		i;

	i = 0;
	count_arg = 0;
	if (!node->args)
		return ;
	while (node->args[count_arg])
		count_arg ++;
	expanded_args = clean_old_content(malloc
			(sizeof(char *) * (count_arg + 1)), false);
	if (!expanded_args)
	{
		printf("expansion args malloc fail\n");
		return ;
	}
	while (node->args[i])
	{
		expanded = expand_word(node->args[i], shell_program);
		if (!expanded)
		{
			printf("Error: expansion failed for argument \n");
			return ;
		}
		expanded_args[i] = expanded;
		i++;
	}
	expanded_args[i] = NULL;
	node->expanded_args = expanded_args;
}

char	*expand_word(char *str, t_shell *shell_program)
{
	int		i;
	char	*tmp;
	char	*resultat;

	i = 0;
	resultat = clean_old_content(ft_strdup(""), false);
	while (str[i])
	{
		if (str[i] == '\'')
			tmp = content_in_single_quote(str, &i);
		else if (str[i] == '"')
			tmp = content_in_double_quote(str, &i, shell_program);
		else if (str[i] == '$')
			tmp = content_with_variable(str, &i, shell_program);
		else
			tmp = content_simple(str, &i);
		resultat = clean_old_content(ft_strjoin(resultat, tmp), false);
	}
	return (resultat);
}

char	*content_in_single_quote(char *str, int *i)
{
	int		start;
	char	*content;

	(*i)++;
	start = *i;
	while (str[*i] && str[*i] != '\'')
		(*i)++;
	content = clean_old_content(ft_substr(str, start, *i - start), false);
	if (str[*i] == '\'')
		(*i)++;
	return (content);
}

char	*content_in_double_quote(char *str, int *i, t_shell *shell_program)
{
	char	*tmp;
	char	*resultat;
	int		start;

	(*i)++;
	resultat = clean_old_content(ft_strdup(""), false);
	while (str[*i] && str[*i] != '"')
	{
		if (str[*i] == '$')
			tmp = content_with_variable(str, i, shell_program);
		else
		{
			start = *i;
			while (str[*i] && str[*i] != '"' && str[*i] != '$')
				(*i)++;
			tmp = clean_old_content(ft_substr(str, start, *i - start), false);
		}
		resultat = clean_old_content(ft_strjoin(resultat, tmp), false);
	}
	if (str[*i] == '"')
		(*i)++;
	return (resultat);
}

char	*getenv_value(t_shell *shell_program, const char *var_name)
{
	t_env	*envlist;

	envlist = shell_program->envlist;
	while (envlist)
	{
		if (ft_strcmp(envlist->key, var_name) == 0)
		{
			return (envlist->value);
		}
		envlist = envlist->next;
	}
	return (NULL);
}
