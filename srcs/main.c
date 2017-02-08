#include "libft.h"
#include "printf.h"
#include "strerror.h"

#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <linux/icmp.h>

/*
** context context
*/
# define MAXADDRSIZE	64
# define MAXPACKET		4096

typedef struct	s_context
{
	char			*host;
	struct addrinfo	addrinfo;
	char			addr[MAXADDRSIZE];
	int				maxwait;
	int				verbose;
	unsigned char	packet[MAXPACKET];
}				t_context;

/*
** icmp checksum
*/
__sum16	compute_checksum(__u8 *buf, size_t size)
{
	__u64	sum;
	size_t	i;

	sum = 0;
	i = 0;
	while (i < size)
	{
		sum += *(__u16 *)buf;
		buf += 2;
		i += 2;
	}
	if (size - i > 0)
		sum += *(__u8 *)buf;
	while ((sum >> 16) != 0)
		sum = (sum & 0xffff) + (sum >> 16);
	return ((__u16)~sum);
}

/*
** open connection
*/
struct addrinfo	*gethostaddrinfo(const char *host)
{
	struct addrinfo	hints;
	struct addrinfo	*head;
	int				err;

	ft_bzero(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_RAW;
	hints.ai_protocol = IPPROTO_ICMP;
	err = getaddrinfo(host, NULL, &hints, &head);
	if (err)
	{
		ft_fprintf(2, "getaddrinfo: %s\n", gai_strerror(err));
		return (NULL);
	}
	return (head);
}

#include <arpa/inet.h>

int			get_addrstr(const struct addrinfo *addrinfo,
						char *addrstr)
{
	const char	*res;

	res = inet_ntop(AF_INET,
					&((struct sockaddr_in *)addrinfo->ai_addr)->sin_addr,
					addrstr,
					MAXADDRSIZE);
	if (res == NULL)
	{
		perror("inet_ntop", errno);
		return (EXIT_FAILURE);
	}
	return (0);
}

int			open_connection(const char *host,
								struct addrinfo *addrinfo)
{
	struct addrinfo	*head;
	struct addrinfo	*pos;
	int				sockfd;

	head = gethostaddrinfo(host);
	if (head == NULL)
		return (-1);
	pos = head;
	while (pos != NULL)
	{
		sockfd = socket(pos->ai_family,
						pos->ai_socktype,
						pos->ai_protocol);
		if (sockfd >= 0)
			break ;
		perror("socket", errno);
		pos = pos->ai_next;
	}
	if (pos == NULL)
		return (-1);
	*addrinfo = *pos;
	freeaddrinfo(head);
	return (sockfd);
}

void	setup_echorequest(struct icmphdr *icmp)
{
	ft_bzero(icmp, sizeof(*icmp));
	icmp->type = ICMP_ECHO;
	icmp->code = 0;
	icmp->checksum = 0;
	icmp->un.echo.id = getpid() & 0xffff;
	icmp->un.echo.sequence = 0;
}

/*
** launch request
*/
void			send_echorequest(const int sock,
								struct addrinfo *addrinfo,
								struct icmphdr *icmp)
{
	ssize_t			nbytes;

	icmp->un.echo.sequence++;
	icmp->checksum = compute_checksum((void *)icmp, sizeof(*icmp));
	nbytes = sendto(sock,
					icmp,
					sizeof(*icmp),
					0,
					addrinfo->ai_addr,
					addrinfo->ai_addrlen);
	if (nbytes < 0)
	{
		perror("sendto", errno);
		exit(EXIT_FAILURE);
	}
}

int		ft_ping(t_context *context)
{
	int				sock;
	struct icmphdr	request;
	struct icmphdr	reply;
	int				err;

	sock = open_connection(context->host, &context->addrinfo);
	if (sock < 0)
		return (1);
	err = get_addrstr(&context->addrinfo, context->addr);
	if (err)
		return (1);
	setup_echorequest(&request);
	while (1)
	{
		ft_printf("Send ICMP echo request to %s\n", context->addr);
		send_echorequest(sock, &context->addrinfo, &request);
		
	}
	close(sock);
	return (0);
}

/*
** Init
*/
static void	default_context(t_context *context)
{
	ft_bzero(context, sizeof(*context));
	context->maxwait = 10;
	context->verbose = 0;
}

static void	usage(char *prog)
{
	ft_fprintf(2, "Usage: %s [-h|-v] ip\n", prog);
}

static char	ft_getopt(char *arg)
{
	if (!arg)
		return (0);
	if (arg[0] != '-')
		return (0);
	if (ft_strlen(arg) != 2)
		return (0);
	return (arg[1]);
}

static int	init_context(int argc,
						char **argv,
						t_context *context)
{
	char	c;
	int		i;

	default_context(context);
	i = 1;
	while ((c = ft_getopt(argv[i])))
	{
		if (c == 'v')
			context->verbose = 1;
		else
			return (1);
		i++;
	}
	if (i != argc - 1)
		return (1);
	context->host = argv[i];
	return (0);
}

int		main(int argc, char **argv)
{
	t_context	context;
	int			err;
	int			exit_status;

	err = init_context(argc, argv, &context);
	if (err)
	{
		usage(argv[0]);
		return (EXIT_FAILURE);
	}
 	exit_status = ft_ping(&context);
	return (exit_status);
}
