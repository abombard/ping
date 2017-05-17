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

uint16_t	compute_checksum(uint8_t *buf, unsigned int size)
{
	uint64_t		sum;
	unsigned int	i;

	sum = 0;
	i = 0;
	while (i < size)
	{
		sum += *(uint16_t *)buf;
		buf += sizeof(uint16_t);
		i += sizeof(uint16_t);
	}
	if (size - i > 0)
		sum += *(uint8_t *)buf;
	while ((sum >> 16) != 0)
		sum = (sum & 0xffff) + (sum >> 16);
	return ((uint16_t)~sum);
}

int			checksum_isvalid(struct icmp *icmp)
{
	uint16_t		checksum;
	uint16_t		expected_checksum;

	checksum = icmp->icmp_cksum;
	icmp->icmp_cksum = 0;
	expected_checksum = compute_checksum((void *)icmp, sizeof(struct icmp));
	return (checksum == expected_checksum);
}
