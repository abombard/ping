/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checksum.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/06 14:50:43 by abombard          #+#    #+#             */
/*   Updated: 2017/05/06 14:50:51 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ping.h"

__sum16	compute_checksum(__u8 *buf, unsigned int size)
{
	__u64			sum;
	unsigned int	i;

	sum = 0;
	i = 0;
	while (i < size)
	{
		sum += *(__u16 *)buf;
		buf += sizeof(__u16);
		i += sizeof(__u16);
	}
	if (size - i > 0)
		sum += *(__u8 *)buf;
	while ((sum >> 16) != 0)
		sum = (sum & 0xffff) + (sum >> 16);
	return ((__u16)~sum);
}

int		checksum_isvalid(struct icmphdr *icmp)
{
	__sum16		checksum;
	__sum16		expected_checksum;

	checksum = icmp->checksum;
	icmp->checksum = 0;
	expected_checksum = compute_checksum((void *)icmp, sizeof(struct icmphdr));
	return (checksum == expected_checksum);
}
