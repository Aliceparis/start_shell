/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yujin <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 15:31:56 by yujin             #+#    #+#             */
/*   Updated: 2024/11/15 13:17:53 by yujin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	int		i;
	int		len_s;
	char	*res;

	i = 0;
	len_s = 0;
	while (s[len_s] != '\0')
		len_s++;
	res = (char *)malloc(sizeof(char) * (len_s + 1));
	if (res == NULL)
		return (NULL);
	while (s[i] != '\0')
	{
		res[i] = s[i];
		i++;
	}
	res[i] = '\0';
	return (res);
}
