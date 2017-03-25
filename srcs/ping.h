#ifndef PING_H
# define PING_H

# include "libft.h"

# include <sys/socket.h>
# include <sys/time.h>
# include <sys/signal.h>

# include <netinet/ip.h>
# include <linux/icmp.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <errno.h>
# include <limits.h>

/* # define DEBUG */

# define PROGNAME		"ft_ping"

# define ADDRFAMILY		AF_INET
# define MAXADDRSIZE	64

# define ICMP_MINLEN	28

# define DATALEN		(64 - 8)
# define MAXIPLEN		60
# define MAXICMPLEN		76
# define MAXPACKET		(65536 - 60 - 8)
# define MAXWAIT		10

# define MAX_DUP_CK	(8 * 128)

struct
{
	char						hostname[MAXADDRSIZE];
	char						hostaddr[MAXADDRSIZE];

	struct sockaddr_storage		sockaddr;
	socklen_t					sockaddrlen;

	int							sockfd;

	char						recv_table[MAX_DUP_CK / 8];

#ifdef DEBUG
	u_char						outpack[MAXPACKET];
#endif

	int							ident;

	long						npackets;
	long						nreceived;
	long						nrepeats;
	long						ntransmitted;
	int							interval;

	int							timing;
	long						tmin;
	long						tmax;
	u_long						tsum;

	int							verbose;

} g_context;

# define A(bit)		g_context.recv_table[(bit) >> 3]
# define B(bit)		(1 << ((bit) & 0x07))
# define SET(bit)	(A(bit) |= B(bit))
# define CLR(bit)	(A(bit) &= (~B(bit)))
# define TST(bit)	(A(bit) & B(bit))

/*
** init
*/
void					usage(char *prog);
int						getopts(int argc, char **argv);

/*
** checksum
*/
__sum16					compute_checksum(__u8 *buf, unsigned int size);

/*
** ping
*/
void					init(int argc, char **argv);
void					pinger(void);

void					catcher(int signum);
void					finish(int signum);

#endif
