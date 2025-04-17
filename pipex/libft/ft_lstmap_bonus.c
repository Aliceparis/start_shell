/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yujin <yujin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 14:31:48 by yujin             #+#    #+#             */
/*   Updated: 2024/11/19 09:51:37 by yujin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*res;
	t_list	*new;
	void	*new_content;

	res = NULL;
	while (lst != NULL)
	{
		new_content = f(lst->content);
		if (new_content == NULL)
		{
			ft_lstclear(&res, del);
			return (NULL);
		}
		new = ft_lstnew(new_content);
		if (new == NULL)
		{
			del(new_content);
			ft_lstclear(&res, del);
			return (NULL);
		}
		ft_lstadd_back(&res, new);
		lst = lst->next;
	}
	return (res);
}
