/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmen <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 15:02:54 by jmen              #+#    #+#             */
/*   Updated: 2024/11/18 14:50:54 by jmen             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *nptr)
{
	int	signe;
	int	resultat;

	signe = 1;
	while (*nptr && (*nptr == ' ' || (*nptr >= 9 && *nptr <= 13)))
		nptr++;
	if (*nptr && (*nptr == '-' || *nptr == '+'))
	{
		if (*nptr == '-')
			signe = signe * -1;
		nptr++;
	}
	resultat = 0;
	while (*nptr && (*nptr >= '0' && *nptr <= '9'))
	{
		resultat = resultat * 10 + *nptr - '0';
		nptr++;
	}
	return (resultat * signe);
}
/*
int	main()
{
	printf("%d\n", atoi("-47-5"));
	printf("%d\n", ft_atoi("-47-5"));

}*/
