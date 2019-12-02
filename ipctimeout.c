/*
**	I P C T I M E O U T
**
**	Copyright 1990, 1991, 1992, 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		IPC Timeout
**	Program		libipc
**	Project		IPC Library
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Fri Jun 22 17:08:09 MDT 1990
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
**
**	Abstract
**
**	This function is part of a low-level interprocess communication
**	package. Higher-level application-specific layers will probably
**	be built on top of this.
*/

static char copyright[]="Copyright 1990, 1991, 1992, 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)ipctimeout.c	4.2 94/05/16 jsloan@ncar.ucar.edu";

#include "libipc.h"
#ifdef SELECT_LIBIPC
#include <sys/select.h>
#endif /* SELECT_LIBIPC */

/*
**	ipctimeout: timeout on on a socket waiting for data to
**	arrive.
**
**	Arguments:
**		sock	file descriptor
**		secs	number of seconds to timeout
**
**	Returns:	>0 if data available within timeout period
**			=0 if interrupted
**			<0 if timeout or other error
*/
int
ipctimeout(int sock, int secs)
	{
	struct timeval tout;
	fd_set mask;
	int count;

	FD_ZERO(&mask);
	FD_SET(sock,&mask);

	tout.tv_sec=secs;
	tout.tv_usec=0;

	if ((count=select(sock+1,&mask,((fd_set *)0),((fd_set *)0),&tout))==0)
		{
		seterrno(ETIMEDOUT);
		return(-2);
		}

	else if ((count<0)&&(geterrno()==EINTR))
		return(0);

	else if (count<0)
		return(-3);

	else if (count>1)
		{
		seterrno(ERANGE);
		return(-4);
		}

	else if (!FD_ISSET(sock,&mask))
		{
		seterrno(ERANGE);
		return(-5);
		}

	return(1);
	}
