/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yujin <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 15:45:28 by yujin             #+#    #+#             */
/*   Updated: 2024/11/09 15:49:05 by yujin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t n)
{
	size_t	len_d;
	size_t	len_s;
	size_t	i;

	len_d = 0;
	len_s = 0;
	while (dest[len_d] != '\0' && len_d < n)
		len_d++;
	while (src[len_s] != '\0')
		len_s++;
	if (n <= len_d)
		return (n + len_s);
	i = 0;
	while (src[i] != '\0' && (len_d + i) < (n - 1))
	{
		dest[len_d + i] = src[i];
		i++;
	}
	if ((len_d + i) < n)
		dest[len_d + i] = '\0';
	return (len_d + len_s);
}
