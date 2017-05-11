/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_list_add.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <abombard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/01 00:00:21 by abombard          #+#    #+#             */
/*   Updated: 2016/01/01 00:00:42 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "list.h"

void	t_list_add(t_list *new, t_list *prev, t_list *next)
{
	next->prev = new;
	new->next = next;
	new->prev = prev;
	prev->next = new;
}
