/*
**	I P C R E A D Y
**
**	Copyright 1989, 1990, 1991, 1992, 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		IPC Ready
**	Program		libipc
**	Project		IPC Library
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Tue Oct 10 08:59:30 MDT 1989
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
**
**	Abstract
**
**	This function is part of a low-level interprocess communication
**	package. Higher-level application-specific layers will probably
**	be built on top of this.
*/

static char copyright[]="Copyright 1989, 1990, 1991, 1992, 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)ipcready.c	4.2 94/05/16 jsloan@ncar.ucar.edu";

#include "libipc.h"
#ifdef SELECT_LIBIPC
#include <sys/select.h>
#endif /* SELECT_LIBIPC */

#define NS	((fd_set *)0)

static int ipc_count;		/* thread safe: sem_ipcready_libi */
static int ipc_first;		/* thread safe: sem_ipcready_libi */
static int ipc_last;		/* thread safe: sem_ipcready_libi */
static fd_set ipc_mask;		/* thread safe: sem_ipcready_libi */
static fd_set ipc_ready;	/* thread safe: sem_ipcready_libi */
static int initial=1;		/* thread safe: sem_ipcready_libi */

static void
initialize(void)
	{
	ipc_count=0;
	ipc_first=0;
	ipc_last=0;
	FD_ZERO(&ipc_mask);
	FD_ZERO(&ipc_ready);
	initial=0;
	}

/*
**	ipcregister: registers a socket so that it is recognized
**	by ipcready and its ilk.
*/
void
ipcregister(IPCSOCKET sock)
	{
	P(sem_ipcready_libi);
	if (initial)
		initialize();
	FD_SET(sock,&ipc_mask);
	if (sock>ipc_last)
		ipc_last=sock;
	V(sem_ipcready_libi);
	}

/*
**	ipcunregister: remove a socket from consideration by
**	ipcready and its kin.
*/
void
ipcunregister(IPCSOCKET sock)
	{
	P(sem_ipcready_libi);
	if (initial)
		initialize();
	FD_CLR(sock,&ipc_mask);
	V(sem_ipcready_libi);
	}

/*
**	ipcderegister: performs ipcunregister against all
**	registered sockets.
*/
void
ipcderegister(void)
	{
	P(sem_ipcready_libi);
	if (initial)
		initialize();
	FD_ZERO(&ipc_mask);
	ipc_last=0;
	V(sem_ipcready_libi);
	}

/*
**	ipcdump: dumps internal data for debugging.
*/
void
ipcdump(FILE *fp)
	{
	P(sem_ipcready_libi);

	fprintf(fp,"ipcready: ipc_count=%d, ipc_first=%d, ipc_last=%d\n",
		ipc_count,ipc_first,ipc_last);

	fprintf(fp,
	"ipcready: ipc_mask=0x%lx 0x%lx 0x%lx 0x%lx 0x%lx 0x%lx 0x%lx 0x%lx\n",
		ipc_mask.fds_bits[7],
		ipc_mask.fds_bits[6],
		ipc_mask.fds_bits[5],
		ipc_mask.fds_bits[4],
		ipc_mask.fds_bits[3],
		ipc_mask.fds_bits[2],
		ipc_mask.fds_bits[1],
		ipc_mask.fds_bits[0]);

	fprintf(fp,
	"ipcready: ipc_ready=0x%lx 0x%lx 0x%lx 0x%lx 0x%lx 0x%lx 0x%lx 0x%lx\n",
		ipc_ready.fds_bits[7],
		ipc_ready.fds_bits[6],
		ipc_ready.fds_bits[5],
		ipc_ready.fds_bits[4],
		ipc_ready.fds_bits[3],
		ipc_ready.fds_bits[2],
		ipc_ready.fds_bits[1],
		ipc_ready.fds_bits[0]);

	V(sem_ipcready_libi);
	}

/*
**	ipcready: return the next ipcaccept'ed socket that has
**	data waiting to be read. Block until ready;
**
**	ipcpoll: Non-blocking ipcready().
**
**	ipctimed: blocking ipcready() with timeout interval
**
**	Arguments:
**		seconds	(ipctimed only) timeout period
**
**	Returns:	next ready socket, <0 if error
**
**	N.B.	We prevent starvation by servicing all pending
**		requests before selecting a new set of requests.
**		This may provide slow service on the busier
**		ports, but the round robin scheduling insures
**		that everyone gets a turn.
*/
static IPCSOCKET
ipcselect(struct timeval *delay)
	{
	int sock;

	P(sem_ipcready_libi);

	/*
	** If ipc_count==0, then we've exhausted the previous
	** list of ready sockets and its time to generate a
	** new list.
	**
	** If ipc_count<0 then the previous select
	** failed (probably interrupted by a signal) and the
	** list was never completed; hence we need to generate
	** a new one again.
	*/

	if (ipc_count<=0)
		{

		ipc_first=0;

		bcopy(&ipc_mask,&ipc_ready,sizeof(ipc_ready));

		if ((ipc_count=select(ipc_last+1,&ipc_ready,NS,NS,delay))<0)
			{
			V(sem_ipcready_libi);
			return(-2);
			}

		else if (ipc_count==0)
			{
			V(sem_ipcready_libi);
			seterrno(ETIMEDOUT);
			return(-1);
			}

		}

	for (sock=ipc_first; sock<=ipc_last; sock++)
		if (FD_ISSET(sock,&ipc_ready))
			{
			FD_CLR(sock,&ipc_ready);
			ipc_count--;
			ipc_first=sock+1;
			V(sem_ipcready_libi);
			return(sock);
			}

	V(sem_ipcready_libi);
	seterrno(ERANGE);

	return(-3);
	}

IPCSOCKET
ipcready(void)
	{
	return(ipcselect((struct timeval *)0));
	}

IPCSOCKET
ipctimed(int seconds)
	{
	struct timeval timeout;

	timeout.tv_sec=seconds;
	timeout.tv_usec=0;
	return(ipcselect(&timeout));
	}

IPCSOCKET
ipcpoll(void)
	{
	return(ipctimed(0));
	}
