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

static void			icmp_init(struct icmphdr *icmp)
{
	icmp->type = ICMP_ECHO;
	icmp->code = 0;
	icmp->checksum = 0;
	icmp->un.echo.sequence = ++g_context.ntransmitted;
	icmp->un.echo.id = g_context.ident;
}

void				pinger(void)
{
	u_char			packet[MAXPACKET];
	int				packlen;
	int				i;
	int				nbytes;

	ft_bzero(packet, MAXPACKET);
	icmp_init((struct icmphdr *)packet);
	(void)gettimeofday((struct timeval *)(packet + sizeof(struct icmphdr)),
		(struct timezone *)0);
	packlen = DATALEN + 8;
	icmp->checksum = compute_checksum((void *)packet, packlen);
	g_context.recv_table[icmp->un.echo.sequence % MAX_DUP_CK] = 0;
	i = 0;
	while (i < packlen)
	{
		nbytes = sendto(g_context.sockfd, (char *)packet, packlen, 0,
				(struct sockaddr *)&g_context.sockaddr, g_context.sockaddrlen);
		if (nbytes < 0)
		{
			perror(PROGNAME ": sendto");
			return ;
		}
		i += nbytes;
	}
}
