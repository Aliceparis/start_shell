/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmen <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 13:03:10 by jmen              #+#    #+#             */
/*   Updated: 2024/11/18 13:55:28 by jmen             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*debut;
	t_list	*occupe;

	if (!lst || !f || !del)
		return (NULL);
	debut = ft_lstnew(f(lst->content));
	occupe = debut;
	while (lst && lst->next)
	{
		if (!occupe)
			return (NULL);
		occupe->next = ft_lstnew(f(lst->next->content));
		occupe = occupe->next;
		lst = lst->next;
	}
	return (debut);
}
