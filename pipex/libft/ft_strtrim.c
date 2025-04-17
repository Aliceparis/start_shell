/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yujin <yujin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 09:26:32 by yujin             #+#    #+#             */
/*   Updated: 2024/11/18 13:28:50 by yujin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_instr(char c, char const *set)
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

char	*ft_strtrim(char const *s1, char const *set)
{
	int		i;
	char	*res;
	int		start;
	int		end;

	i = 0;
	start = 0;
	while (ft_instr(s1[start], set) == 1 && s1[start])
		start++;
	end = ft_strlen(s1);
	while (ft_instr(s1[end - 1], set) == 1 && end > start)
		end--;
	res = (char *)malloc(sizeof(char) * (end - start + 1));
	if (res == NULL)
		return (NULL);
	while (start < end)
	{
		res[i] = s1[start];
		i++;
		start++;
	}
	res[i] = '\0';
	return (res);
}
/*
int	main(int ac, char *av[])
{
	(void)ac;
	printf ("%s\n",ft_strtrim(av[1], av[2]));
	return (0);
}*/