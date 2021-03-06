/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   finish.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/06 14:50:59 by abombard          #+#    #+#             */
/*   Updated: 2017/05/06 14:52:55 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ping.h"

void			finish(int signum)
{
	(void)signum;
	(void)signal(SIGINT, SIG_IGN);
	printf("\n--- %s ping statistics ---\n", g_context.hostname);
	printf("%ld packets transmitted, ", g_context.ntransmitted);
	printf("%ld packets received, ", g_context.nreceived);
	if (g_context.nrepeats)
		(void)printf("+%ld duplicates, ", g_context.nrepeats);
	if (g_context.ntransmitted)
	{
		if (g_context.nreceived > g_context.ntransmitted)
			(void)printf("-- somebody's printing up packets!");
		else
			(void)printf("%d%% packet loss", (int)(((g_context.ntransmitted -
					g_context.nreceived) * 100) / g_context.ntransmitted));
	}
	printf("\n");
	if (g_context.nreceived)
	{
		(void)printf("round-trip min/avg/max = %ld.%ld/%lu.%ld/%ld.%ld ms\n",
			g_context.tmin / 10, g_context.tmin % 10,
			(g_context.tsum / (g_context.nreceived + g_context.nrepeats)) / 10,
			(g_context.tsum / (g_context.nreceived + g_context.nrepeats)) % 10,
			g_context.tmax / 10, g_context.tmax % 10);
	}
	exit(g_context.nreceived ? EXIT_SUCCESS : EXIT_FAILURE);
}
