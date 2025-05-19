#include <stdio.h>
char	*get_key_env(char *str)
{
	int	i = 0;

	while (str[i])
	{
		if (str[i] == '=')
			return (ft_substr(str, 0, i));
		i++;
	}
	return (ft_strdup(str));
}
char	*get_value_env(char *str)
{
	int	i = 0;
	char	*resultat = NULL;

	while (str[i])
	{
		if (str[i] == '=')
		{
			i++;
			resultat = ft_substr(str, i, ft_strlen(str) - i);
		}
		i++;
	}
	return (resultat);
}
void	init_envlist(char **envp)
{
	int	i = 0;
	char	*key;
	char	*value;

	while (envp[i])
	{
		key = get_key_env(envp[i]);
		value = get_value_env(envp[i]);
		update();
		i++;
	}
}

int	main(int ac, char **av, char **envp)
{

}

