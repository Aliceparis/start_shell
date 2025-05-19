/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmen <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 17:26:59 by jmen              #+#    #+#             */
/*   Updated: 2024/11/21 17:31:25 by jmen             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	in_string(char const *set, char c)
{
	int	i;

	i = 0;
	while (set[i] != '\0')
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s, char const*set)
{
	int		i;
	int		start;
	int		end;
	char	*resultat;

	i = 0;
	start = 0;
	while (in_string(set, s[start]) == 1 && s[start])
		start++;
	end = ft_strlen(s);
	while (in_string(set, s[end - 1]) == 1 && end > start)
		end--;
	resultat = (char *)malloc(sizeof(char) * (end - start + 1));
	if (!resultat)
		return (NULL);
	while (start < end)
	{
		resultat[i] = s[start];
		i++;
		start++;
	}
	resultat[i] = '\0';
	return (resultat);
}
