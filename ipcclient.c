/*
**	I P C C L I E N T
**
**	Copyright 1989, 1990, 1991, 1994, 1995 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		IPC Client
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
static char sccsid[]="@(#)ipcclient.c	4.2 95/07/26 jsloan@ncar.ucar.edu";

#include "libipc.h"

/*
**	ipcclient: establish a client connection to an existing
**	stream socket at the specified address and port.
**
**	Arguments:
**		address	internet address (in binary network
			byte order, as from ipcaddress)
**		port	port number (in binary network byte
**			order, as from ipcport)
**
**	Returns:	connected socket file descriptor, or
**			<0 if error.
*/
IPCSOCKET
ipcclient(IPCADDRESS address, IPCPORT port)
	{
	struct sockaddr_in internet;
	IPCSOCKET sock;

	bzero((char *)&internet,sizeof(internet));
	internet.sin_addr.s_addr=address;
	internet.sin_family=AF_INET;
	internet.sin_port=port;
	if ((sock=socket(AF_INET,SOCK_STREAM,0))<0)
		return(sock);
	if (connect(sock,(struct sockaddr *)&internet,sizeof(internet))<0)
		{
		close(sock);
		return(-2);
		}
	ipcregister(sock);
	return(sock);
	}
