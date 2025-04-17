#include "minishell.h"

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
char	*expande_word(char *str)
{
	int	i;
	char	*resultat;
	int	start = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '\'')
		{
			resultat = content_in_single_quote(str);
			continu ;
		}
		else if (str[i] == '"')
		{
			resultat = content_in_double_quote(str);
		}
	}
}

char	*content_in_single_quote(char *str)
{
	int	i;
	int	start;
	char	*tmp;
	char	*resultat;

	i = 0;
	resultat = ft_strdup("");
	while (str[i])
	{
		i++;
		start = i;
		while (str[i] != '\'')
			i++;
		tmp = ft_substr(str, start, (i - start));
		resultat = ft_strjoin(resultat, tmp);
		free(tmp);
		i++;
	}
	return (resultat);
}
char	*content_in_double_quote(char *str)
{
	int	i;
	int	start;
	int	var_start;
	int	var_len;
	char	*tmp;
	char	*resultat;
	char	*var_name;
	char	*value;
	i = 0;
	i++;
	start = i;
	while (str[i] && str[i] != '"')
	{
		var_start = i + 1;
		while (ft_isalnum(str[var_start + var_len]) || str[var_start + var_len] == '_')
			var_len++;
		var_name = strndup(str+ var_start, var_len);
		value = getenv(var_name);
		if (value)
			tmp = ft_substr(str, var_start, var_len);
		
	}
}
