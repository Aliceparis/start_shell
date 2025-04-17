/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yujin <yujin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 13:29:31 by yujin             #+#    #+#             */
/*   Updated: 2024/11/18 14:10:25 by yujin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	affiche(long nb, int fd)
{
	if (nb / 10)
	{
		affiche(nb / 10, fd);
		affiche(nb % 10, fd);
	}
	else
		ft_putchar_fd(nb + '0', fd);
}

void	ft_putnbr_fd(int n, int fd)
{
	long	nb;

	nb = n;
	if (fd < 0)
		return ;
	if (nb < 0)
	{
		write(fd, "-", 1);
		nb = -nb;
	}
	affiche(nb, fd);
}
