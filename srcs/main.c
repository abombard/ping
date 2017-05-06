/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/06 15:06:12 by abombard          #+#    #+#             */
/*   Updated: 2017/05/06 15:08:34 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ping.h"

#ifdef DEBUG

void	ip_dump(struct iphdr *ip)
{
	printf("ip v %d size %d\n", ip->version, ip->ihl << 2);
	printf("tos %d tot_len %d id %d frag_off %d ttl %d protocol %d check %d\n",
			ip->tos, ip->tot_len, ip->id, ip->frag_off, ip->ttl, ip->protocol, ip->check);
	printf("saddr %s daddr %s\n", inet_ntoa(*(struct in_addr *)&ip->saddr), inet_ntoa(*(struct in_addr *)&ip->daddr));
}

#endif

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

void	unpack_icmph_echoreply(struct icmphdr *icmp,
		int *id, int *seq, int *dup)
{
	*id = icmp->un.echo.id;
	*seq = icmp->un.echo.sequence;
	if (g_context.recv_table[icmp->un.echo.sequence % MAX_DUP_CK] != 0)
	{
		g_context.nrepeats += 1;
		g_context.nreceived -= 1;
		*dup = 1;
	}
	else
	{
		g_context.recv_table[icmp->un.echo.sequence % MAX_DUP_CK] = 1;
		*dup = 0;
	}
}

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

void	treat_icmp_echoreply(struct icmphdr *icmp, int packlen, int ttl)
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
	triptime = round_triptime(tp);

	(void)printf("%d bytes from %s: icmp_seq=%u", packlen,
		g_context.hostname, seq);
	(void)printf(" ttl=%d", ttl);
	(void)printf(" time=%ld.%ld ms", triptime / 10, triptime % 10);
	if (dup)
		(void)printf(" (DUP!)");

	printf("\n");
}

void	unpack(char *packet, int packlen)
{
	struct icmphdr	*icmp;
	int				hlen;
	int				ttl;
	int				type;
	int				code;

	//fprintf(stderr, "DATALEN %d sizeof iphdr %lu icmphdr %lu timveal %lu\n",
	//		DATALEN, sizeof(struct iphdr), sizeof(struct icmphdr), sizeof(struct timeval));

	if ((unsigned long)packlen <
		sizeof(struct iphdr) + sizeof(struct icmphdr) + sizeof(struct timeval))
	{
		if (g_context.verbose)
			(void)fprintf(stderr, PROGNAME ": packet too short (%d bytes) from %s\n",
				packlen, g_context.hostaddr);
		return ;
	}

	unpack_iph((struct iphdr *)packet, &hlen, &ttl);

	packlen -= hlen;

	icmp = (struct icmphdr *)(packet + hlen);
	unpack_icmph(icmp, &type, &code);

	if (type == ICMP_ECHOREPLY)
	{
		treat_icmp_echoreply(icmp, packlen, ttl);
	}
	else if (g_context.verbose)
	{
		(void)printf("%d bytes from %s: ", packlen,
			g_context.hostname);
		(void)printf("type=%d, code=%d", type, code);

		printf("\n");
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
