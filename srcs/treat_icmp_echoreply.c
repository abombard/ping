/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   treat_icmp_echoreply.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/09 14:02:08 by abombard          #+#    #+#             */
/*   Updated: 2017/05/09 14:02:09 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ping.h"

static void	unpack_icmph_echoreply(struct icmp *icmp,
		int *id, int *seq, int *dup)
{
	*id = icmp->icmp_id;
	*seq = icmp->icmp_seq;
	if (g_context.recv_table[icmp->icmp_seq % MAX_DUP_CK] != 0)
	{
		g_context.nrepeats += 1;
		g_context.nreceived -= 1;
		*dup = 1;
	}
	else
	{
		g_context.recv_table[icmp->icmp_seq % MAX_DUP_CK] = 1;
		*dup = 0;
	}
}

void		treat_icmp_echoreply(struct icmp *icmp, int packlen, int ttl)
{
	int				id;
	int				seq;
	int				dup;
	struct timeval	*tp;
	long			triptime;

	unpack_icmph_echoreply(icmp, &id, &seq, &dup);
	if (id != g_context.ident)
		return ;
	g_context.nreceived += 1;
	tp = (struct timeval *)(icmp + 1);
	printf("%d bytes from %s (%s): icmp_seq=%u", packlen,
		g_context.truehostname, g_context.hostaddr, seq);
	printf(" ttl=%d", ttl);
	triptime = round_triptime(tp) / 2;
	printf(" time=%ld.%ld ms", triptime / 100, triptime % 100);
	if (dup)
		printf(" (DUP!)");
	printf("\n");
}
