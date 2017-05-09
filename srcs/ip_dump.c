/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ip_dump.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/09 14:04:44 by abombard          #+#    #+#             */
/*   Updated: 2017/05/09 14:04:45 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ping.h"

void	ip_dump(struct iphdr *ip)
{
	printf("ip v %d size %d\n", ip->version, ip->ihl << 2);
	printf("tos %d tot_len %d id %d frag_off %d ttl %d protocol %d check %d\n",
			ip->tos, ip->tot_len, ip->id, ip->frag_off, ip->ttl, ip->protocol,
			ip->check);
	printf("saddr %s daddr %s\n", inet_ntoa(*(struct in_addr *)&ip->saddr),
			inet_ntoa(*(struct in_addr *)&ip->daddr));
}
