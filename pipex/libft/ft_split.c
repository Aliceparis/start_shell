/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yujin <yujin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 12:31:41 by yujin             #+#    #+#             */
/*   Updated: 2024/11/19 09:39:55 by yujin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_count_words(char const *str, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		while (str[i] && str[i] == c)
			i++;
		if (str[i])
			count++;
		while (str[i] && str[i] != c)
			i++;
	}
	return (count);
}

static char	*ft_copy_range(char const *str, int start, int end)
{
	char	*result;
	int		i;

	result = (char *)malloc(sizeof(char) * (end - start + 1));
	if (!result)
		return (NULL);
	i = 0;
	while (start < end)
		result[i++] = str[start++];
	result[i] = '\0';
	return (result);
}

static void	*ft_free_tab(char **tab, int l)
{
	while (l > 0)
		free(tab[--l]);
	free(tab);
	return (NULL);
}

static char	**ft_split_words(char const *s, char c, char **tab)
{
	int	i;
	int	start;
	int	l;

	i = 0;
	l = 0;
	while (s[i])
	{
		while (s[i] && s[i] == c)
			i++;
		start = i;
		while (s[i] && s[i] != c)
			i++;
		if (i > start)
		{
			tab[l] = ft_copy_range(s, start, i);
			if (!tab[l])
				return (ft_free_tab(tab, l));
			l++;
		}
	}
	tab[l] = NULL;
	return (tab);
}

char	**ft_split(char const *s, char c)
{
	char	**tab;

	if (!s)
		return (NULL);
	tab = (char **)malloc(sizeof(char *) * (ft_count_words(s, c) + 1));
	if (!tab)
		return (NULL);
	return (ft_split_words(s, c, tab));
}
/*
int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		printf("Usage: %s <string> <separator>\n", argv[0]);
		return (1);
	}

	char **result = ft_split(argv[1], *argv[2]);
	int i = 0;
	while (result[i] != NULL)
	{
		printf("Word %d: %s\n", i, result[i]);
		free(result[i]);
		i++;
	}
	free(result);

	return (0);
}*/
