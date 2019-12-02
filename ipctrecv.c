/*
**	I P C T R E C V
**
**	Copyright 1989, 1990, 1991, 1992, 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		IPC Fixed-Length Receive with Timeout
**	Program		libipc
**	Project		IPC Library
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Wed Oct 25 13:42:45 MDT 1989
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
**
**	Abstract
**
**	This function is part of a low-level interprocess communication
**	package. Higher-level application-specific layers will probably
**	be built on top of this.
*/

static char copyright[]="Copyright 1989, 1990, 1991, 1992, 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)ipctrecv.c	4.1 94/05/16 jsloan@ncar.ucar.edu";

#include "libipc.h"

/*
**	ipctrecv: receive a fixed length message. Timeout if the
**	message isn't received in time.
**
**	Arguments:
**		sock	connected stream socket
**		buffer	address of buffer
**		length	length of requested message
**		secs	seconds to wait for timeout
**
**	Returns:	number of bytes received (==length),
**			0 if connection gone, <0 if error
*/
int
ipctrecv(IPCSOCKET sock, char *buffer, int length, int secs)
	{
	int l, n, rc;

	if (buffer==NULL)
		{
		seterrno(EINVAL);
		return(-6);
		}

	for (n=0; n<length; n+=l)
		{

		do
			rc=ipctimeout(sock,secs);
		while (rc==0);

		if (rc<0)
			return(rc);

		if ((l=IPCRECV(sock,buffer+n,length-n))<=0)
			return(l);
		}

	return(n);
	}
