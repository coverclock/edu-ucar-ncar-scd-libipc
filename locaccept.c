/*
**	L O C A C C E P T
**
**	Copyright 1994, 1995 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		Local Accept
**	Program		libipc
**	Project		IPC Library
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Fri Sep 30 10:21:50 MDT 1994
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
**
**	Abstract
**
**	This function is part of a low-level interprocess communication
**	package. Higher-level application-specific layers will probably
**	be built on top of this.
*/

static char copyright[]="Copyright 1994, 1995 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)locaccept.c	1.3 95/08/02 jsloan@ncar.ucar.edu";

#include "libipc.h"

/*
**	locaccept: wait until a connection is requested for the
**	spcified socket. Then return an identical socket to
**	actually handle the transaction.
**
**	Arguments:
**		sock	connected stream socket
**
**	Returns:	new socket or <0 if error
*/
IPCSOCKET
locaccept(IPCSOCKET sock)
	{
	struct sockaddr_un sockpath;
	int length;
	IPCSOCKET newsock;

	length=sizeof(sockpath);
	if ((newsock=accept(sock,(struct sockaddr *)&sockpath,&length))<0)
		return(-1);
	ipcregister(newsock);
	return(newsock);
	}
