/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/06 14:55:42 by abombard          #+#    #+#             */
/*   Updated: 2017/05/06 14:57:30 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

/*
** # define DEBUG
*/

# define PROGNAME		"ft_ping"

# define ADDRFAMILY		AF_INET
# define MAXADDRSIZE	64

# define ICMP_MINLEN	28

# define DATALEN		(64 - 8)
# define MAXIPLEN		60
# define MAXICMPLEN		76
# define MAXPACKET		(65536 - 60 - 8)
# define MAXWAIT		10

# define MAX_DUP_CK		(8 * 128)

struct					s_context
{
	char						hostname[MAXADDRSIZE];
	char						hostaddr[MAXADDRSIZE];

	struct sockaddr_storage		sockaddr;
	socklen_t					sockaddrlen;

	int							sockfd;

	char						recv_table[MAX_DUP_CK / 8];

	int							ident;

	long						npackets;
	long						nreceived;
	long						nrepeats;
	long						ntransmitted;
	int							interval;

	long						tmin;
	long						tmax;
	u_long						tsum;

	int							verbose;

	int							ttl;

}						g_context;

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
** tvsub
*/
void					tvsub(struct timeval *out, struct timeval *in);

/*
** ping
*/
void					init(int argc, char **argv);
void					gethostaddr(const int ai_family,
								const struct sockaddr *sockaddr);

void					pinger(void);
void					catcher(int signum);
void					finish(int signum);

#endif
