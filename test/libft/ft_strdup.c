/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmen <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 15:14:24 by jmen              #+#    #+#             */
/*   Updated: 2024/11/12 15:42:59 by jmen             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	char	*resultat;
	int		i;

	resultat = (char *)malloc(sizeof(char) * (ft_strlen(s) + 1));
	if (resultat == NULL)
		return (NULL);
	i = 0;
	while (s[i])
	{
		resultat[i] = s[i];
		i++;
	}
	resultat[i] = '\0';
	return (resultat);
}
