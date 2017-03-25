#include "ping.h"

void	catcher(int signum)
{
	int		waittime;

	(void)signum;
	pinger();
	(void)signal(SIGALRM, catcher);
	if (!g_context.npackets || g_context.ntransmitted < g_context.npackets)
		alarm((u_int)g_context.interval);
	else
	{
		if (g_context.nreceived)
		{
			waittime = 2 * g_context.tmax / 1000;
			if (!waittime)
				waittime = 1;
			if (waittime > MAXWAIT)
				waittime = MAXWAIT;
		}
		else
			waittime = MAXWAIT;
		(void)signal(SIGALRM, finish);
		(void)alarm((u_int)waittime);
	}
}
