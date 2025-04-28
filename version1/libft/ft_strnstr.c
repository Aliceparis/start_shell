/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmen <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 14:43:52 by jmen              #+#    #+#             */
/*   Updated: 2024/11/12 15:02:30 by jmen             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *dest, const char *to_find, size_t len)
{
	size_t	i;
	size_t	len_src;
	size_t	j;

	i = 0;
	if (ft_strlen(to_find) == 0)
		return ((char *)dest);
	len_src = ft_strlen(to_find);
	while (i < len && dest[i])
	{
		j = 0;
		while (dest[i + j] && dest[i + j] == to_find[j] && i + j < len)
			j++;
		if (j == len_src)
			return ((char *)dest + i);
		i++;
	}
	return (0);
}
