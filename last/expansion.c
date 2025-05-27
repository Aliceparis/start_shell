#include "minishell.h"

/*!!!!!!expand_comande + ft_expand_ast non test*/
void	ft_expand_ast(ASTnode *node, t_shell *shell_program)
{
	if (!node)
	{
		error_message(shell_program, "ast est nulle\n", 2);
		reset_terminal(shell_program);
	}
	else
	{
		if (node->type == CMD)
			expande_commande(node, shell_program);
		else if (node->type == PIPE)
		{
			ft_expand_ast(node->left, shell_program);
			ft_expand_ast(node->right, shell_program);
		}
		else
			ft_expand_ast(node->left, shell_program);
	}
}

void	expande_commande(ASTnode *node, t_shell *shell_program)
{
	char	*expanded;
	int	i;

	i = 0;
	while (node->args[i])
	{
		expanded = expand_word(node->args[i], shell_program);
		if (!expanded)  // 处理扩展失败的情况
        {
            printf("Error: expansion failed for argument \n");
            return;
        }
		//free(node->args[i]);
		node->args[i] = expanded;
		i++;
	}
}

/*如果遇到了单引号，那么直接去掉单引号，然后现实内容
如果遇到了双引号，$后面的variable需要进行getenv来替换
？？？？？  $？ 的使用还需要添加在内*/

char	*expand_word(char *str, t_shell *shell_program)
{
	int	i;
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
		//printf("tmp expand_word :%s\n", tmp);
		resultat =clean_old_content(ft_strjoin(resultat, tmp), false);
	}
	return (resultat);
}
char	*content_in_single_quote(char *str, int *i)
{
	int	start;
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
	int	start;

	(*i)++; // double_quote所在地方
	resultat = clean_old_content(ft_strdup(""), false);
	while (str[*i] && str[*i] != '"')
	{
		if (str[*i] == '$')
			tmp = content_with_variable(str, i, shell_program);
		else /*juste need to move_out the double quote liKe in single quote*/
		{
			start = *i;
			while (str[*i] && str[*i] != '"' && str[*i] != '$')
				(*i)++;
			tmp =clean_old_content(ft_substr(str, start, *i - start), false);
		}
		resultat = clean_old_content(ft_strjoin(resultat, tmp), false);
		//free(tmp);	
	}
	if (str[*i] == '"')
		(*i)++;
	return (resultat);
}

char	*content_with_variable(char *str, int *i, t_shell *shell_program)
{
	int	start;
	char	*resultat;
	char	*var_name;
	int		var_len;
	char	*tmp;

	if (str[*i + 1] == '?')
	{
		*i += 2;
		return (clean_old_content(ft_itoa(shell_program->exit_status), false));
	}
	//// fonction pour retourner la valeur de la derniere execution 
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
	tmp = getenv(var_name);
	free(var_name);
	if (tmp)
		resultat = clean_old_content(ft_strjoin(resultat, tmp), false);
	*i = start + var_len;
	return (resultat);
}

char	*content_simple(char *str, int *i)
{
	int	j;
	char	*resultat;
	j = *i;
	while (str[*i] && str[*i] != '\'' && str[*i] != '"' && str[*i] != '$')
		(*i)++;
	resultat = clean_old_content(ft_substr(str, j, *i -j), false);
	return (resultat);
}
/*
char	*joint_and_free(char *s1, char *s2)
{
	char	*resultat;

	resultat =clean_old_content(ft_strjoin(s1, s2), false);
	free(s1);
	return (resultat);
}*/