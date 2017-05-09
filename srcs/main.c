/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/06 15:06:12 by abombard          #+#    #+#             */
/*   Updated: 2017/05/09 14:03:32 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ping.h"

void	unpack_iph(struct iphdr *ip, int *hlen, int *ttl)
{
	*hlen = ip->ihl << 2;
	*ttl = ip->ttl;
}

void	unpack_icmph(struct icmphdr *icmp,
					int *type, int *code)
{
	*type = icmp->type;
	*code = icmp->code;
}

void	unpack(char *packet, int packlen)
{
	struct icmphdr	*icmp;
	int				hlen;
	int				ttl;
	int				type;
	int				code;

	if ((unsigned long)packlen <
		sizeof(struct iphdr) + sizeof(struct icmphdr) + sizeof(struct timeval))
	{
		if (g_context.verbose)
			(void)fprintf(stderr, PROGNAME
	": packet too short (%d bytes) from %s\n", packlen, g_context.hostaddr);
		return ;
	}
	unpack_iph((struct iphdr *)packet, &hlen, &ttl);
	packlen -= hlen;
	icmp = (struct icmphdr *)(packet + hlen);
	unpack_icmph(icmp, &type, &code);
	if (type == ICMP_ECHOREPLY)
		treat_icmp_echoreply(icmp, packlen, ttl);
	else if (g_context.verbose)
	{
		printf("%d bytes from %s: type=%d, code=%d\n", packlen,
			g_context.hostname, type, code);
	}
}

int		recv_icmp(void)
{
	char				packet[MAXPACKET];
	int					packlen;
	struct sockaddr_in	from;
	socklen_t			fromlen;

	fromlen = sizeof(from);
	packlen = recvfrom(g_context.sockfd,
		(char *)packet, sizeof(packet), 0,
		(struct sockaddr *)&from, &fromlen);
	if (packlen < 0)
	{
		if (errno == EINTR)
			return (1);
		perror(PROGNAME ": recvfrom");
		return (1);
	}
	unpack(packet, packlen);
	return (!(g_context.npackets && g_context.nreceived >= g_context.npackets));
}

int		main(int argc, char **argv)
{
	int		live;

	init(argc, argv);
	signal(SIGINT, finish);
	signal(SIGALRM, catcher);
	(void)printf("PING %s (%s): %d data bytes\n",
		g_context.hostname, g_context.hostaddr, DATALEN);
	catcher(0);
	live = 1;
	while (live)
		live = recv_icmp();
	finish(0);
	return (0);
}
