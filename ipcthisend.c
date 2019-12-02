/*
**	I P C T H I S E N D
**
**	Copyright 1989, 1990, 1991, 1994, 1995 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		IPC This End
**	Program		TAGS libipc
**	Project		Text And Graphics System
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Thu Nov  2 15:38:35 MST 1989
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
**
**	Abstract
**
**	This function is part of a low-level interprocess communication
**	package. Higher-level application-specific layers will probably
**	be built on top of this. This is not TAGS specific.
*/

static char copyright[]="Copyright 1989, 1990, 1991, 1994, 1995 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)ipcthisend.c	4.3 95/08/02 jsloan@ncar.ucar.edu";

#include "libipc.h"

/*
**	ipcthisend: given a connected socket locate the corresponding
**	address and port for the local end.
**
**	Arguments:
**		sock	connected stream socket
**		addrp	result parm
**		portp	result parm
**
**	Returns:	0 if successful, <0 otherwise
*/
int
ipcthisend(IPCSOCKET sock, IPCADDRESS *addrp, IPCPORT *portp)
	{
	struct sockaddr_in internet;
	int length;

	length=(sizeof(struct sockaddr_in));
	if (getsockname(sock,(struct sockaddr *)&internet,&length)<0)
		return(-1);
	if (addrp!=NULL)
		*addrp=internet.sin_addr.s_addr;
	if (portp!=NULL)
		*portp=internet.sin_port;
	return(0);
	}
