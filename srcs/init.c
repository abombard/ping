#include "ping.h"

static void		gethostaddrv4(const struct sockaddr *sockaddr)
{
	struct sockaddr_in	*sockaddr_in;

	sockaddr_in = (struct sockaddr_in *)sockaddr;
	inet_ntop(AF_INET, (void *)&sockaddr_in->sin_addr,
			g_context.hostaddr, sizeof(g_context.hostaddr));
}

static void		gethostaddrv6(const struct sockaddr *sockaddr)
{
	struct sockaddr_in6	*sockaddr_in6;

	sockaddr_in6 = (struct sockaddr_in6 *)sockaddr;
	inet_ntop(AF_INET6, (void *)&sockaddr_in6->sin6_addr,
			g_context.hostaddr, sizeof(g_context.hostaddr));
}

static void		gethostaddr(const int ai_family, const struct sockaddr *sockaddr)
{
	if (ai_family == AF_INET)
		gethostaddrv4(sockaddr);
	else if (ai_family == AF_INET6)
		gethostaddrv6(sockaddr);
	else
	{
		fprintf(stderr, PROGNAME ": gethostaddr: ai_family %d is not handled\n", ai_family);
		exit(EXIT_FAILURE);
	}
}

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
	g_context.timing = 1;
	g_context.interval = 1;
	g_context.tmin = LONG_MAX;
	g_context.tmax = 0;
	g_context.ident = getpid() & 0xffff;
	getopts(argc, argv);
	gethostaddrinfo(g_context.hostname);
}

