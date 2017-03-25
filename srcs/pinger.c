#include "ping.h"

void				pinger(void)
{
#ifdef DEBUG
	u_char			*packet = g_context.outpack;
#else
	u_char			packet[MAXPACKET];
#endif
	struct icmphdr	*icmp;
	int				cc;
	int				i;
	int				nbytes;

	ft_bzero(packet, MAXPACKET);
	icmp = (struct icmphdr *)packet;
	icmp->type = ICMP_ECHO;
	icmp->code = 0;
	icmp->checksum = 0;
	icmp->un.echo.sequence = ++g_context.ntransmitted;
	icmp->un.echo.id = g_context.ident;

	CLR(icmp->un.echo.sequence % MAX_DUP_CK);
	if (g_context.timing)
	{
		(void)gettimeofday((struct timeval *)(packet + sizeof(struct icmphdr)),
			(struct timezone *)0);
	}
	cc = DATALEN + 8;

	icmp->checksum = compute_checksum((void *)icmp, cc);

	i = 0;
	while (i < cc)
	{
		nbytes = sendto(g_context.sockfd, (char *)packet, cc, 0,
				(struct sockaddr *)&g_context.sockaddr, g_context.sockaddrlen);
		if (nbytes < 0)
		{
			perror(PROGNAME ": sendto");
			return ;
		}
		i += nbytes;
	}
}
