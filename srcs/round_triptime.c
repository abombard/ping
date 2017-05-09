/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   round_triptime.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/09 14:01:57 by abombard          #+#    #+#             */
/*   Updated: 2017/05/09 14:01:58 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ping.h"

long	round_triptime(struct timeval *tp)
{
	struct timeval	tv;
	long			triptime;

	(void)gettimeofday(&tv, (struct timezone *)0);
	tvsub(&tv, tp);
	triptime = tv.tv_sec * 10000 + (tv.tv_usec / 100);
	g_context.tsum += triptime;
	if (triptime < g_context.tmin)
		g_context.tmin = triptime;
	if (triptime > g_context.tmax)
		g_context.tmax = triptime;
	return (triptime);
}
