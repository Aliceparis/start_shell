/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yujin <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 12:34:11 by yujin             #+#    #+#             */
/*   Updated: 2024/11/14 12:28:47 by yujin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char *))
{
	unsigned int	i;
	unsigned int	len_s;

	i = 0;
	len_s = ft_strlen(s);
	if (s != NULL && f != NULL)
	{
		while (i < len_s)
		{
			(*f)(i, &s[i]);
			i++;
		}
	}
}
