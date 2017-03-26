#include <stdio.h>

void	usage(char *prog)
{
	fprintf(stderr, "Usage: %s [-h|-v|-t <ttl>|-c <packet count>] ip\n", prog);
}
