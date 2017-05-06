/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getopts.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/06 14:53:03 by abombard          #+#    #+#             */
/*   Updated: 2017/05/06 14:54:51 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ping.h"
#include "libft.h"

#include <stdlib.h>

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

static int	is_digit(char *arg)
{
	if (!arg)
		return (0);
	return (ft_atoi(arg));
}

int			getopts(int argc, char **argv)
{
	char	c;
	int		i;

	i = 0;
	while ((c = ft_getopt(argv[++i])))
	{
		if (c == 'v')
			g_context.verbose = 1;
		else if (c == 'c' && is_digit(argv[i + 1]) > 0)
			g_context.npackets = ft_atoi(argv[i++ + 1]);
		else if (c == 't' && is_digit(argv[i + 1]) > 0)
			g_context.ttl = ft_atoi(argv[i++ + 1]);
		else
		{
			usage(argv[0]);
			exit(EXIT_FAILURE);
		}
	}
	if (i != argc - 1)
	{
		usage(argv[0]);
		exit(EXIT_FAILURE);
	}
	ft_strncpy(g_context.hostname, argv[i], sizeof(g_context.hostname));
	return (0);
}
