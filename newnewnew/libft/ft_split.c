/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmen <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 16:52:28 by jmen              #+#    #+#             */
/*   Updated: 2024/11/21 17:08:48 by jmen             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	size_t	get_string(char const *s, char c)
{
	size_t	i;
	size_t	count;

	i = 0;
	count = 0;
	while (s[i])
	{
		if (s[i] != c && (s[i + 1] == c || s[i + 1] == '\0'))
			count++;
		i++;
	}
	return (count);
}

static	void	ft_cpy(char const*s, char c, char *sep)
{
	size_t	i;

	i = 0;
	while (s[i] && s[i] != c)
	{
		sep[i] = s[i];
		i++;
	}
	sep[i] = '\0';
}

static void	*ft_free(char **str)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
	return (NULL);
}

static char	**sep_string(char const*s, char c, char **new)
{
	size_t	index;
	size_t	i;
	size_t	start;

	i = 0;
	start = 0;
	index = 0;
	while (s[i])
	{
		while (s[i] && s[i] == c)
			i++;
		start = i;
		while ((s[i] != c) && s[i])
			i++;
		if (i > start)
		{
			new[index] = malloc(sizeof(char) * (i - start + 1));
			if (!new)
				return (ft_free(new));
			ft_cpy(&s[start], c, new[index]);
			index++;
		}
	}
	new[index] = NULL;
	return (new);
}

char	**ft_split(char const *s, char c)
{
	char	**new;
	size_t	size;

	size = get_string(s, c);
	new = (char **)malloc(sizeof(char *) * (size + 1));
	if (!new)
		return (NULL);
	sep_string(s, c, new);
	return (new);
}
