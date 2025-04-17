/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yujin <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 14:40:42 by yujin             #+#    #+#             */
/*   Updated: 2024/11/15 13:16:07 by yujin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t			i;
	size_t			total;
	unsigned char	*res;

	i = 0;
	total = nmemb * size;
	if (nmemb != 0 && total / nmemb != size)
		return (NULL);
	res = (void *)malloc(sizeof(void) * total);
	if (res == NULL)
		return (NULL);
	while (i < total)
	{
		res[i] = 0;
		i++;
	}
	return ((void *)res);
}
