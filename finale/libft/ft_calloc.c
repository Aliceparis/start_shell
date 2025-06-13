/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmen <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 15:18:50 by jmen              #+#    #+#             */
/*   Updated: 2024/11/13 15:06:35 by jmen             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	*ft_malloc(size_t size)
{
	void	*new;
	size_t	i;

	new = (void *)malloc(size);
	if (!new)
		return (NULL);
	i = 0;
	while (i < size)
	{
		*(unsigned char *)(new + i) = 0;
		i++;
	}
	return (new);
}

void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t	total;

	total = size * nmemb;
	if (!size && total / nmemb != size)
		return (NULL);
	return (ft_malloc(nmemb * size));
}
