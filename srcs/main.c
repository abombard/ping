#include "ping.h"

void	tvsub(struct timeval *out, struct timeval *in)
{
	out->tv_usec -= in->tv_usec;
	if (out->tv_usec < 0)
	{
		--out->tv_sec;
		out->tv_usec += 1000000;
	}
	out->tv_sec -= in->tv_sec;
}

void	display(struct iphdr *ip, struct icmphdr *icmp, int cc, long triptime, int dupflag)
{
	(void)printf("%d bytes from %s: icmp_seq=%u", cc,
		g_context.hostname, icmp->un.echo.sequence);
	(void)printf(" ttl=%d", ip->ttl);
	if (g_context.timing)
		(void)printf(" time=%ld.%ld ms", triptime / 10, triptime % 10);
	if (dupflag)
		(void)printf(" (DUP!)");

#ifdef DEBUG
	u_char	*cp;
	u_char	*dp;
	int		i;

	cp = ((u_char *)(icmp + 1) + 8);
	dp = &g_context.outpack[8 + sizeof(struct timeval)];

	i = 8;
	while (i < DATALEN)
	{
		if (*cp != *dp)
		{
			printf("\nwrong data byte #%d should be 0x%x but was 0x%x", i, *dp, *cp);
			cp = (u_char *)(icmp + 1);

			i = 8;
			while (i < DATALEN)
			{
				if (i % 32 == 8)
					(void)printf("\n\t");
				(void)printf("%x ", *cp);
				++i, ++cp;
			}
			break ;
		}
		++i, ++cp, ++dp;
	}
#endif

}

#ifdef DEBUG

void	ip_dump(struct iphdr *ip)
{
	printf("ip v %d size %d\n", ip->version, ip->ihl << 2);
	printf("tos %d tot_len %d id %d frag_off %d ttl %d protocol %d check %d\n",
			ip->tos, ip->tot_len, ip->id, ip->frag_off, ip->ttl, ip->protocol, ip->check);
	printf("saddr %s daddr %s\n", inet_ntoa(*(struct in_addr *)&ip->saddr), inet_ntoa(*(struct in_addr *)&ip->daddr));
}

#endif

void	pr_pack(char *buf, int cc, struct sockaddr_in *from, socklen_t fromlen)
{
	struct icmphdr	*icmp;

	/*
	u_long			l;
	int			j;
	static int				old_rrlen;
	static char				old_rr[MAX_IPOPTLEN];
	*/

	struct iphdr			*ip;
	struct timeval			tv;
	struct timeval			*tp;
	long					triptime;
	int						hlen;
	int						dupflag;

	(void)from;
	(void)fromlen;

	triptime = 0;

	(void)gettimeofday(&tv, (struct timezone *)0);

	ip = (struct iphdr *)buf;
	hlen = ip->ihl << 2;
	if (cc < DATALEN + ICMP_MINLEN)
	{
		if (g_context.verbose)
		{
			(void)fprintf(stderr, PROGNAME ": packet too short (%d bytes) from %s\n",
				cc, g_context.hostaddr);
		}
		return ;
	}

	cc -= hlen;
	icmp = (struct icmphdr *)(buf + hlen);
	if (icmp->type == ICMP_ECHOREPLY)
	{
		if (icmp->un.echo.id != g_context.ident)
			return ;
		++g_context.nreceived;
		if (g_context.timing)
		{
			tp = (struct timeval *)(icmp + 1);
			tvsub(&tv, tp);
			triptime = tv.tv_sec * 10000 + (tv.tv_usec / 100);
			g_context.tsum += triptime;
			if (triptime < g_context.tmin)
				g_context.tmin = triptime;
			if (triptime > g_context.tmax)
				g_context.tmax = triptime;
		}

		if (TST(icmp->un.echo.sequence % MAX_DUP_CK))
		{
			++g_context.nrepeats;
			--g_context.nreceived;
			dupflag = 1;
		}
		else
		{
			SET(icmp->un.echo.sequence % MAX_DUP_CK);
			dupflag = 0;
		}

		display(ip, icmp, cc, triptime, dupflag);

	}
	else
	{
		if (g_context.verbose)
		{
			(void)printf("%d bytes, from %s: ", cc, g_context.hostaddr);
			(void)printf("type=%d code=%d\n", icmp->type, icmp->code);
		}
	}
	printf("\n");
}

int		loop(void)
{
	u_char				packet[DATALEN + MAXIPLEN + MAXICMPLEN];
	struct sockaddr_in	from;
	socklen_t			fromlen;
	int					cc;

	fromlen = sizeof(from);
	cc = recvfrom(g_context.sockfd,
		(char *)packet, sizeof(packet), 0,
		(struct sockaddr *)&from, &fromlen);
	if (cc < 0)
	{
		if (errno == EINTR)
			return (1);
		perror(PROGNAME ": recvfrom");
		return (1);
	}
	pr_pack((char *)packet, cc, &from, fromlen);
	return (!(g_context.npackets && g_context.nreceived >= g_context.npackets));
}

int		main(int argc, char **argv)
{
	init(argc, argv);

	signal(SIGINT, finish);
	signal(SIGALRM, catcher);

	/* -- ? -- */
	int		hold;

	hold = 48 * 1024;
	(void)setsockopt(g_context.sockfd, SOL_SOCKET, SO_RCVBUF, (char *)&hold, sizeof(hold));

	/* -- PING -- */

	(void)printf("PING %s (%s): %d data bytes\n",
		g_context.hostname, g_context.hostaddr, DATALEN);

	catcher(0);
	while (loop())
		;
	finish(0);

	return (0);
}
