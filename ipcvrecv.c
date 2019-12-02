/*
**	I P C V R E C V
**
**	Copyright 1989, 1990, 1991, 1992, 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		IPC Variable-Length Blocking Receive
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
static char sccsid[]="@(#)ipcvrecv.c	4.1 94/05/16 jsloan@ncar.ucar.edu";

#include "libipc.h"

/*
**	ipcvrecv: receive a variable length message. Return whatever
**	is received with a single request.
**
**	Arguments:
**		sock	connected stream socket
**		buffer	address of buffer
**		length	length of requested message
**
**	Returns:	number of bytes received (<=length) or
**			<0 if error
**
**	N.B.	It isn't necessary to have a timeout version of
**		this. If mready/select says something is there,
**		then at least something is available. The timeout
**		is necessary for the fixed ipcrecv since we may
**		have to do multiple recvs, even though the
**		mready/select only guarantees that there is something
**		there for the first recv to see.
*/
int
ipcvrecv(IPCSOCKET sock, char *buffer, int length)
	{
	if (buffer==NULL)
		{
		seterrno(EINVAL);
		return(-2);
		}
	return(IPCRECV(sock,buffer,length));
	}
