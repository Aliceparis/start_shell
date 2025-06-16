/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmen <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 16:45:56 by jmen              #+#    #+#             */
/*   Updated: 2024/11/19 16:50:40 by jmen             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, char const *src, size_t destsize)
{
	size_t	len_dest;
	size_t	len_src;
	size_t	i;

	i = 0;
	len_dest = ft_strlen(dest);
	len_src = ft_strlen(src);
	if (destsize == 0 || destsize <= len_dest)
		return (destsize + len_src);
	while (src[i] && i + len_dest < destsize - 1)
	{
		dest[len_dest + i] = src[i];
		i++;
	}
	dest[len_dest + i] = '\0';
	return (len_src + len_dest);
}
