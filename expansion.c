#include "minishell.h"
/*!!!!!!expand_comande + ft_expand_ast non test*/
void	ft_expand_ast(ASTnode *node)
{
	ASTnode	*tmp;

	tmp = node;
	if (!node)
	////fonction error and free (error : )
	while (node)
	{
		if (node->type == CMD)
		{
			expande_commande(node);
			break ;
		}
		else if (node->type == PIPE)
		{
			ft_expand_ast(node->left);
			ft_expand_ast(node->right);
			break;
		}
		else if (node->type == REDIRECTION)
			ft_expand_ast(node->left);
		if (node->file)
			expande_word(node->file);
	}
}
void	expande_commande(ASTnode *node)
{
	char	*expanded;
	int	i;

	i = 0;
	while (node->args[i])
	{
		expanded = expande_word(node->args[i]);
		free(node->args[i]);
		node->args[i] = expanded;
		i++;
	}
}

/*如果遇到了单引号，那么直接去掉单引号，然后现实内容
如果遇到了双引号，$后面的variable需要进行getenv来替换
？？？？？  $？ 的使用还需要添加在内*/

char	*change_variable(char *str, int *i)
{
	int	var_start;
	char	*var_name;
	char	*tmp;
	char	*value;
	int	var_len = 0;

	(*i)++;
	var_start = *i;
	value = ft_strdup("");
	if (str[var_start] == '?')
	////fonction pour retourner la valeur de la derniere execution
		return (NULL);
	while (ft_isalnum(str[var_start + var_len]) || str[var_start + var_len] == '_')
		var_len++;
	var_name = ft_substr(str, var_start, var_len);
	tmp = getenv(var_name);
	if (tmp)
		value = ft_strjoin(value, tmp);
	//free(tmp);
	*i = var_start + var_len;
	return (value);
}

char	*content_in_single_quote(char *str, int *i)
{
	int	start;
	char	*content;

	(*i)++;
	start = *i;
	while (str[*i] && str[*i] != '\'')
		(*i)++;
	content = ft_substr(str, start, *i - start);
	if (str[*i] == '\'')
		(*i)++;
	return (content);
}

char	*content_in_double_quote(char *str, int *i)
{
	char	*tmp;
	const char	*content;
	char	*resultat;
	int	j;
	int	start;

	(*i)++; // double_quote所在地方
	j = *i;
	resultat = ft_strdup("");
	while (str[j] && str[j] != '"')
	{
		if (str[j] == '$')
			tmp = change_variable(str, &j);
		else /*juste need to move_out the double quote liKe in single quote*/
		{
			start = j;
			while (str[j] && str[j] != '"')
				j++;
			tmp = ft_substr(str, start, j - start);
		}		
	}
	resultat = ft_strjoin(resultat, tmp);
	free(tmp);
	if (str[j] == '"')
		j++;
	*i = j;
	return (resultat);
}


char	*expande_word(char *str)
{
	int	i;
	char	*tmp;
	char	*resultat;
	const char	*content;
	int	j;

	i = 0;
	resultat = ft_strdup("");
	while (str[i])
	{
		if (str[i] == '\'')
			tmp = content_in_single_quote(str, &i);
		else if (str[i] == '"')
			tmp = content_in_double_quote(str, &i);	
		else if (str[i] == '$')
			tmp = change_variable(str, &i);
		else
		{
			j = i;
			while (str[i] && str[i] !='\'' && str[i] != '"' && str[i] !='$')
				i++;
			tmp = ft_substr(str, j, i - j);
		}
		resultat = ft_strjoin(resultat, tmp);
		free(tmp);
	}
	return (resultat);
}
