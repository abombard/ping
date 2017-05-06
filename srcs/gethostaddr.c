/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gethostaddr.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/06 14:49:58 by abombard          #+#    #+#             */
/*   Updated: 2017/05/06 14:50:35 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

extern void		gethostaddr(const int ai_family,
					const struct sockaddr *sockaddr)
{
	if (ai_family == AF_INET)
		gethostaddrv4(sockaddr);
	else if (ai_family == AF_INET6)
		gethostaddrv6(sockaddr);
	else
	{
		fprintf(stderr, PROGNAME ": gethostaddr: ai_family %d is not handled\n",
				ai_family);
		exit(EXIT_FAILURE);
	}
}
