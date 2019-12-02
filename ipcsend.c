/*
**	I P C S E N D
**
**	Copyright 1989, 1990, 1991, 1992, 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		IPC Send
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
static char sccsid[]="@(#)ipcsend.c	4.1 94/05/16 jsloan@ncar.ucar.edu";

#include "libipc.h"

/*
**	ipcsend: send a buffer of the specified length. Continue
**	sending until the entire buffer is sent (generally it will
**	all be sent the first time).
**
**	Arguments:
**		sock	connected stream socket
**		buffer	address of buffer
**		length	length of buffer
**
**	Returns:	number of bytes sent,
**			0 if connection gone,
**			<0 if error
*/
int
ipcsend(IPCSOCKET sock, char *buffer, int length)
	{
	int l, n;

	if (buffer==NULL)
		{
		seterrno(EINVAL);
		return(-1);
		}

	for (n=0; n<length; n+=l)
		if ((l=IPCSEND(sock,buffer+n,length-n))<=0)
			return(l);

	return(n);
	}
