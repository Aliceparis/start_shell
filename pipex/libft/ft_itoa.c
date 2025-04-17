/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yujin <yujin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 13:28:31 by yujin             #+#    #+#             */
/*   Updated: 2024/11/18 13:46:33 by yujin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_size(int n)
{
	int				res;
	unsigned int	nb;

	res = 0;
	if (n == 0)
		return (1);
	if (n < 0)
	{
		res++;
		nb = n * -1;
	}
	else
		nb = n;
	while (nb > 0)
	{
		res++;
		nb = nb / 10;
	}
	return (res);
}

char	*ft_itoa(int n)
{
	int				len_num;
	int				i;
	unsigned int	nbr;
	char			*res;

	len_num = count_size(n);
	nbr = n;
	i = 0;
	res = (char *)malloc(sizeof(char) * (len_num + 1));
	if (res == NULL)
		return (NULL);
	res[len_num] = '\0';
	if (n < 0)
	{
		nbr = -nbr;
		res[0] = '-';
		i++;
	}
	while (len_num > i)
	{
		len_num--;
		res[len_num] = (nbr % 10) + '0';
		nbr = nbr / 10;
	}
	return (res);
}
/*
int	main(int ac, char *av[])
{
	(void)ac;
	printf("%s\n", ft_itoa(atoi(av[1])));
}*/