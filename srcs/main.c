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
	int				sock;
	struct addrinfo	addrinfo;
	char			addrstr[MAXADDRSIZE];
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
** send request
*/
int			send_echorequest(const int sock,
							struct addrinfo *addrinfo,
							struct icmphdr *icmp)
{
	ssize_t			nbytes;

	nbytes = sendto(sock,
					icmp,
					sizeof(*icmp),
					0,
					addrinfo->ai_addr,
					addrinfo->ai_addrlen);
	if (nbytes < 0)
		return (errno);
	return (0);
}

/*
** read reply
*/
#include <sys/time.h>
void			cur_time_to_usec(long *utime)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	*utime = tv.tv_sec * 1000000L + tv.tv_usec;
}

#define REQUEST_TIMEOUT		1000000
#define REQUEST_INTERVAL	1000000

#include <netinet/ip.h>
int			read_echoreply(const int sock, const __be16 id, struct icmphdr *reply, long *delay)
{
	__u8			rbuf[sizeof(struct ip) + sizeof(struct icmphdr)];
	struct ip		ip;
	long			start_time;
	long			cur_time;
	ssize_t			nbytes;

	cur_time_to_usec(&start_time);
	while (1)
	{
		cur_time_to_usec(&cur_time);
		*delay = cur_time - start_time;

		ft_printf("recvfrom\n");
		nbytes = recvfrom(sock, &rbuf, sizeof(rbuf), 0, NULL, NULL);
		ft_printf("Ok\n");

		if (nbytes < 0)
		{
			if (errno == EAGAIN)
			{
				if (*delay > REQUEST_TIMEOUT)
					return (ETIMEDOUT);
				continue ;
			}
			return (errno);
		}

		ip = *(struct ip *)rbuf;
		*reply = *(struct icmphdr *)((void *)rbuf + sizeof(struct ip));

		if (reply->type == ICMP_ECHOREPLY && reply->un.echo.id == id)
			break ;
	}
	return (0);
}

int		checksum_isvalid(struct icmphdr *icmp)
{
	__sum16		checksum;
	__sum16		expected_checksum;

	checksum = icmp->checksum;
	icmp->checksum = 0;
	expected_checksum = compute_checksum((void *)icmp, sizeof(*icmp));
	return (checksum == expected_checksum);
}

int		ping_loop(t_context *context)
{
	int				seq;
	struct icmphdr	request;
	struct icmphdr	reply;
	long			delay;
	int				err;

	setup_echorequest(&request);
	seq = 0;
	while (++seq)
	{
		request.un.echo.sequence = seq;
		request.checksum = 0;
		request.checksum = compute_checksum((void *)&request, sizeof(request));
		err = send_echorequest(context->sock, &context->addrinfo, &request);
		if (err)
		{
			perror("send_echorequest", err);
			exit(EXIT_FAILURE);
		}
		ft_printf("PING %s (%s)\n", context->host, context->addrstr);
		err = read_echoreply(context->sock, request.un.echo.id, &reply, &delay);
		if (err)
		{
			perror("read_echoreply", err);
			exit(EXIT_FAILURE);
		}
		ft_printf("Received ICMP echo reply from %s: seq=%d, time=%.3f ms\n",
				context->addrstr, reply.un.echo.sequence, delay / 1000.0);
		if (!checksum_isvalid(&reply))
			ft_fprintf(2, "checksum mismatched\n");
		usleep(REQUEST_INTERVAL - delay);
	}
	return (0);
}

/*
** Initialize connection
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
		return (errno);
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

static int	init_connection(const char *host, t_context *context)
{
	int				sock;
	struct addrinfo	addrinfo;
	int				err;

	sock = open_connection(host, &addrinfo);
	if (sock < 0)
		return (1);
	err = get_addrstr(&addrinfo, context->addrstr);
	if (err)
	{
		perror("get_addrstr", err);
		close(sock);
		return (1);
	}
	context->sock = sock;
	context->addrinfo = addrinfo;
	return (0);
}

/*
** Initialize context
*/
static void	default_context(t_context *context)
{
	ft_bzero(context, sizeof(*context));
	context->sock = -1;
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

static int	getopts(int argc,
					char **argv,
					t_context *context)
{
	char	c;
	int		i;

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

	default_context(&context);
	err = getopts(argc, argv, &context);
	if (err)
	{
		usage(argv[0]);
		return (EXIT_FAILURE);
	}
	err = init_connection(context.host, &context);
	if (err)
	{
		ft_fprintf(2, "init_connection failed\n");
		return (EXIT_FAILURE);
	}
 	exit_status = ping_loop(&context);
 	close(context.sock);
	return (exit_status);
}
