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

int			getopts(int argc, char **argv)
{
	char	c;
	int		i;

	i = 1;
	while ((c = ft_getopt(argv[i])))
	{
		if (c == 'v')
			g_context.verbose = 1;
		else if (c == 'c' && argv[i + 1] && ft_atoi(argv[i + 1]) > 0)
		{
			g_context.npackets = ft_atoi(argv[i + 1]);
			++i;
		}
		else if (c == 'h')
		{
			usage(argv[0]);
			exit(EXIT_SUCCESS);
		}
		else
			return (1);
		i++;
	}
	if (i != argc - 1)
		return (1);
	ft_strncpy(g_context.hostname, argv[i], sizeof(g_context.hostname));
	return (0);
}
