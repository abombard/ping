/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_slice.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <abombard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/01 00:00:21 by abombard          #+#    #+#             */
/*   Updated: 2016/01/01 00:00:42 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "list.h"

t_list	*list_slice(t_list *new, t_list *head, const int index, int len)
{
	t_list		*start;
	t_list		*end;
	t_list		*prev;
	t_list		*next;

	if (!new || !head || !len)
		return ((t_list *)0);
	INIT_LIST_HEAD(new);
	start = list_nth(head, index + 1);
	end = list_nth(head, index + len);
	prev = start->prev;
	next = end->next;
	prev->next = next;
	next->prev = prev;
	new->next = start;
	new->prev = end;
	start->prev = new;
	end->next = new;
	return (new);
}
