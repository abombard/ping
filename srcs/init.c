#include "ping.h"

struct addrinfo		*gethostaddressesinfo(const char *hostaddr)
{
	struct addrinfo	*results;
	struct addrinfo	hints;
	int				err;

	ft_bzero(&hints, sizeof(hints));
	hints.ai_family = ADDRFAMILY;
	hints.ai_socktype = SOCK_RAW;
	hints.ai_protocol = IPPROTO_ICMP;
	hints.ai_flags |= AI_CANONNAME;
	err = getaddrinfo(hostaddr, NULL, &hints, &results);
	if (err)
	{
		fprintf(stderr, PROGNAME ": getaddrinfo: %s\n", gai_strerror(err));
		exit(EXIT_FAILURE);
	}
	return (results);
}

static void		gethostaddrinfo(const char *hostaddr)
{
	struct addrinfo		*addresses;
	struct addrinfo		*rp;

	addresses = gethostaddressesinfo(hostaddr);
	ft_strncpy(g_context.hostname, addresses->ai_canonname, sizeof(g_context.hostname));
	rp = addresses;
	while (rp != NULL)
	{
		errno = 0;
		g_context.sockfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (g_context.sockfd >= 0)
			break ;
		if (g_context.verbose && errno)
			perror(PROGNAME ": socket");
		rp = rp->ai_next;
	}
	if (!rp)
	{
		fprintf(stderr, PROGNAME ": failed to connect to the host\n");
		exit(EXIT_FAILURE);
	}
	gethostaddr(rp->ai_family, rp->ai_addr);
	ft_memcpy(&g_context.sockaddr, rp->ai_addr, rp->ai_addrlen);
	g_context.sockaddrlen = rp->ai_addrlen;
	freeaddrinfo(addresses);
}

extern void		init(int argc, char **argv)
{
	ft_bzero(&g_context, sizeof(g_context));
	g_context.interval = 1;
	g_context.tmin = LONG_MAX;
	g_context.tmax = 0;
	g_context.ident = getpid() & 0xffff;
	getopts(argc, argv);
	gethostaddrinfo(g_context.hostname);
	if (g_context.ttl != 0 &&
		setsockopt(g_context.sockfd, IPPROTO_IP, IP_TTL,
				&g_context.ttl, sizeof(g_context.ttl)))
	{
		perror(PROGNAME ": setsockopt ttl");
		exit(EXIT_FAILURE);
	}
}

