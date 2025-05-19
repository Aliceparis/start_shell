/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmen <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 15:33:53 by jmen              #+#    #+#             */
/*   Updated: 2024/11/19 12:46:05 by jmen             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include  "libft.h"

static size_t	get_len(long nb)
{
	size_t	size;

	size = 1;
	if (nb < 0)
	{
		nb = -nb;
		size++;
	}
	while (nb >= 10)
	{
		nb = nb / 10;
		size++;
	}
	return (size);
}

char	*ft_itoa(int n)
{
	long	nbr;
	char	*new;
	size_t	size;
	size_t	debut;

	nbr = (long)n;
	size = get_len(nbr);
	new = (char *)malloc(sizeof(char) * (size + 1));
	if (new == NULL)
		return (NULL);
	debut = 0;
	if (nbr < 0)
	{
		nbr = nbr * -1;
		*(new + 0) = '-';
		debut = 1;
	}
	new[size] = '\0';
	while (size > debut)
	{
		new[size - 1] = nbr % 10 + '0';
		nbr = nbr / 10;
		size--;
	}
	return (new);
}
