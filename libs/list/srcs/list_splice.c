/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_splice.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <abombard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/01 00:00:21 by abombard          #+#    #+#             */
/*   Updated: 2016/01/01 00:00:42 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "list.h"

void	list_splice(t_list *list, t_list *head)
{
	if (!list_is_empty(list))
	{
		t_list_splice(list, head);
		INIT_LIST_HEAD(list);
	}
}
