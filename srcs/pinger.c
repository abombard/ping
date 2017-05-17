/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pinger.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/06 14:57:40 by abombard          #+#    #+#             */
/*   Updated: 2017/05/06 15:00:43 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ping.h"

static int			icmp_init(struct icmp *icmp)
{
	int		packlen;

	icmp->icmp_type = ICMP_ECHO;
	icmp->icmp_code = 0;
	icmp->icmp_cksum = 0;
	icmp->icmp_seq = ++g_context.ntransmitted;
	icmp->icmp_id = g_context.ident;
	(void)gettimeofday((struct timeval *)(icmp + 1),
		(struct timezone *)0);
	packlen = sizeof(struct icmp) + sizeof(struct timeval);
	icmp->icmp_cksum = compute_checksum((void *)icmp, packlen);
	g_context.recv_table[icmp->icmp_seq % MAX_DUP_CK] = 0;
	return (packlen);
}

void				pinger(void)
{
	u_char			packet[MAXPACKET];
	int				packlen;
	int				nbytes;

	ft_bzero(packet, MAXPACKET);
	packlen = icmp_init((struct icmp *)packet);
	nbytes = sendto(g_context.sockfd, (char *)packet, packlen, 0,
			(struct sockaddr *)&g_context.sockaddr, g_context.sockaddrlen);
	if (nbytes < 0 || nbytes != packlen)
	{
		perror(PROGNAME ": sendto");
		return ;
	}
}
