/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flag_apply_field2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <abombard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/01 00:00:21 by abombard          #+#    #+#             */
/*   Updated: 2016/01/01 00:00:42 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "printf.h"

uint8_t	field_right(t_buffer *dst, int len, uint8_t c)
{
	if (len < 0)
		return (0);
	if (dst->size + len > INTERNAL_DATA_SIZE_MAX)
	{
		dst->data[dst->size - 1] = '$';
		return (0);
	}
	ft_memset(dst->data + dst->size, c, len);
	dst->size += len;
	return (1);
}
