#include <stdio.h>
#include <stdlib.h>

int	ft_isalpha(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (1);
	return (0);
}
int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	else
		return (0);
}
size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}
int	ft_isalnum(int c)
{
	if (ft_isalpha(c) == 1 || ft_isdigit(c) == 1)
		return (1);
	else
		return (0);
}
char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*new;
	size_t	i;
	size_t	j;

	new = (char *)malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (new == NULL)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		new[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j])
	{
		new[i + j] = s2[j];
		j++;
	}
	new[i + j] = '\0';
	return (new);
}

char	*ft_strdup(const char *s)
{
	char	*resultat;
	int		i;

	resultat = (char *)malloc(sizeof(char) * (ft_strlen(s) + 1));
	if (resultat == NULL)
		return (NULL);
	i = 0;
	while (s[i])
	{
		resultat[i] = s[i];
		i++;
	}
	resultat[i] = '\0';
	return (resultat);
}
char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char			*new;
	unsigned int	i;

	if (start >= ft_strlen(s))
		len = 0;
	else if (len > ft_strlen(s + start))
		len = ft_strlen(s + start);
	new = (char *)malloc(sizeof(char) * (len + 1));
	if (!new)
		return (NULL);
	i = 0;
	while (start + i < ft_strlen(s) && i < len)
	{
		new[i] = s[start + i];
		i++;
	}
	new[i] = '\0';
	return (new);
}
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
	if (str[var_start] == ' ')
		return ("$");
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
		if (tmp)
			free(tmp);
	}
	return (resultat);
}
int	main(void)
{
	/*char	*test1 = "echo \'\"$USER\"\'";
	char	*test2 = "echo \"\'$HOME\'\"";
	char	*test3 = "\"$cat ls -l\"";*/
	char	*test4 = "Simple $USER$USER$USER$ USER test with \"$USER\" and '$SHELL'\"\"\"";
	
	/*char	*resultat1 = expande_word(test1);
	printf("single quote first : %s\n", resultat1);
	char	*resultat2 = expande_word(test2);
	printf("double quote first: %s\n", resultat2);
	char	*resultat3 = expande_word(test3);
	printf("only $ :%s\n", resultat3);*/
	char	*resultat4 = expande_word(test4);
	printf("only test : %s\n", resultat4);
}