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

void	ip_dump(struct ip *ip)
{
	printf("ip v %d size %d\n", ip->ip_v, ip->ip_hl << 2);
	printf("tos %d tot_len %d id %d frag_off %d ttl %d protocol %d check %d\n",
			ip->ip_tos, ip->ip_len, ip->ip_id, ip->ip_off, ip->ip_ttl, ip->ip_p,
			ip->ip_sum);
	printf("saddr %s daddr %s\n", inet_ntoa(*(struct in_addr *)&ip->ip_src),
			inet_ntoa(*(struct in_addr *)&ip->ip_dst));
}
