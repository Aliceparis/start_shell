/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yujin <yujin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 17:17:17 by yujin             #+#    #+#             */
/*   Updated: 2024/11/18 13:17:23 by yujin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include<stdlib.h>
#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t		i;
	size_t		len_s1;
	char		*res;

	i = 0;
	len_s1 = ft_strlen(s1);
	res = (char *)malloc(sizeof(char) * (len_s1 + ft_strlen(s2) + 1));
	if (res == NULL)
		return (NULL);
	while (s1[i] != '\0')
	{
		res[i] = s1[i];
		i++;
	}
	i = 0;
	while (s2[i] != '\0')
	{
		res[i + len_s1] = s2[i];
		i++;
	}
	res[len_s1 + ft_strlen(s2)] = '\0';
	return (res);
}
/*
int	main(int ac, char *av[])
{
	(void)ac;
	printf("res : %s\n",ft_strjoin("abc", "okok"));  
    printf("res : %s\n",ft_strjoin("ahfad", "okok"));
    printf("res : %s\n",ft_strjoin("abcdesfafs", "okok"));
	printf("res : %s\n",ft_strjoin("absadasff", "ok"));
	printf("%s\n", ft_strjoin(av[1], av[2]));
	return (0);
}*/
