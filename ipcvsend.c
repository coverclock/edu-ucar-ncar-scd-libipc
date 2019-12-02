/*
**	I P C V S E N D
**
**	Copyright 1989, 1990, 1991, 1992, 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		IPC Variable-Length Send
**	Program		libipc
**	Project		IPC Library
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Wed Nov 29 14:09:04 MST 1989
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
**
**	Abstract
**
**	This function is part of a low-level interprocess communication
**	package. Higher-level application-specific layers will probably
**	be built on top of this.
*/

static char copyright[]="Copyright 1989, 1990, 1991, 1992, 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)ipcvsend.c	4.1 94/05/16 jsloan@ncar.ucar.edu";

#include "libipc.h"

/*
**	ipcvsend: send as much of a buffer as possible.
**
**	Arguments:
**		sock	connected stream socket
**		buffer	address of buffer
**		length	length of buffer
**
**	Returns:	number of bytes actually sent,
**			0 if connection lost,
**			<0 if error
*/
int
ipcvsend(IPCSOCKET sock, char *buffer, int length)
	{

	if (buffer==NULL)
		{
		seterrno(EINVAL);
		return(-1);
		}

	return(IPCSEND(sock,buffer,length));
	}
