/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhrima <mhrima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 22:09:09 by mhrima            #+#    #+#             */
/*   Updated: 2022/10/19 14:15:33 by mhrima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*currentlst;
	t_list	*nextlst;

	if (!del || !lst)
		return ;
	currentlst = *lst;
	while (currentlst)
	{
		nextlst = currentlst->next;
		free(currentlst);
		del(currentlst->content);
		currentlst = nextlst;
	}
	*lst = NULL;
}
