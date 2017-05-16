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
	char				*s;

	sockaddr_in = (struct sockaddr_in *)sockaddr;
	s = inet_ntoa(sockaddr_in->sin_addr);
	if (!s)
	{
		perror("inet_ntoa");
		exit(EXIT_FAILURE);
	}
	ft_strncpy(g_context.hostaddr, s, sizeof(g_context.hostaddr));
}

extern void		gethostaddr(const struct sockaddr *sockaddr)
{
	if (sockaddr->sa_family != AF_INET)
	{
		fprintf(stderr, PROGNAME ": gethostaddr: ai_family %d is not handled\n",
				sockaddr->sa_family);
		exit(EXIT_FAILURE);
	}
	gethostaddrv4(sockaddr);
}
