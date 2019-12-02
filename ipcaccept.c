/*
**	I P C A C C E P T
**
**	Copyright 1989, 1990, 1991, 1994, 1995 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		IPC Accept
**	Program		TAGS libipc
**	Project		Text And Graphics System
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Mon Oct  2 16:37:26 MDT 1989
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
**
**	Abstract
**
**	This function is part of a low-level interprocess communication
**	package. Higher-level application-specific layers will probably
**	be built on top of this. This is not TAGS specific.
*/

static char copyright[]="Copyright 1989, 1990, 1991, 1994, 1995 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)ipcaccept.c	4.3 95/08/02 jsloan@ncar.ucar.edu";

#include "libipc.h"

/*
**	ipcaccept: wait until a connection is requested for the
**	spcified socket. Then return an identical socket to
**	actually handle the transaction.
**
**	Arguments:
**		sock	connected stream socket
**
**	Returns:	new socket or <0 if error
*/
IPCSOCKET
ipcaccept(IPCSOCKET sock)
	{
	struct sockaddr_in internet;
	int length;
	IPCSOCKET newsock;

	length=(sizeof(struct sockaddr_in));
	if ((newsock=accept(sock,(struct sockaddr *)&internet,&length))<0)
		return(-1);
	ipcregister(newsock);
	return(newsock);
	}
