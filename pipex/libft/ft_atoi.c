/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yujin <yujin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 12:40:35 by yujin             #+#    #+#             */
/*   Updated: 2024/11/19 09:54:31 by yujin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_isspace(int c)
{
	if (c == ' ' || (c >= '\t' && c <= '\r'))
		return (1);
	return (0);
}

int	ft_atoi(const char *nptr)
{
	int	i;
	int	res;
	int	sign;

	sign = 1;
	i = 0;
	res = 0;
	while (ft_isspace(nptr[i]) == 1)
		i++;
	if (nptr[i] != '+' && nptr[i] != '-' && ft_isdigit(nptr[i]) == 0)
		return (0);
	if (nptr[i] == '-')
		sign = -1;
	if (ft_isdigit(nptr[i]) == 0)
		i++;
	while (ft_isdigit(nptr[i]) == 1)
	{
		res = res * 10 + (nptr[i] - '0');
		i++;
	}
	return (res * sign);
}
