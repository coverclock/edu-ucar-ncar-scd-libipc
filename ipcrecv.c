/*
**	I P C R E C V
**
**	Copyright 1989, 1990, 1991, 1992, 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		IPC Fixed-Length Blocking Receive
**	Program		libipc
**	Project		IPC Library
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Mon Oct  2 16:37:26 MDT 1989
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
**
**	Abstract
**
**	This function is part of a low-level interprocess communication
**	package. Higher-level application-specific layers will probably
**	be built on top of this.
*/

static char copyright[]="Copyright 1989, 1990, 1991, 1992, 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)ipcrecv.c	4.1 94/05/16 jsloan@ncar.ucar.edu";

#include "libipc.h"

/*
**	ipcrecv: receive a fixed length message. Wait until the entire
**	message is received.
**
**	Arguments:
**		sock	connected stream socket
**		buffer	address of buffer
**		length	length of requested message
**
**	Returns:	number of bytes received (==length),
**			0 if connection gone, <0 if error
*/
int
ipcrecv(IPCSOCKET sock, char *buffer, int length)
	{
	int l, n;

	if (buffer==NULL)
		{
		seterrno(EINVAL);
		return(-1);
		}
	for (n=0; n<length; n+=l)
		if ((l=IPCRECV(sock,buffer+n,length-n))<=0)
			return(l);
	return(n);
	}
