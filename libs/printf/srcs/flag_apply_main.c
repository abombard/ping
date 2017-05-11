/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flag_apply_main.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <abombard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/01 00:00:21 by abombard          #+#    #+#             */
/*   Updated: 2016/01/01 00:00:42 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "printf.h"

uint8_t			flag_apply_main(t_buffer *dst, t_flags *flag)
{
	flag_apply_field_precision(dst, flag);
	flag_apply_field_zero(dst, flag);
	if (flag->diez)
		flag_apply_diez(dst, flag->type);
	flag_apply_plus(dst, flag);
	flag_apply_field_space(dst, flag);
	flag_apply_space(dst, flag);
	return (1);
}
