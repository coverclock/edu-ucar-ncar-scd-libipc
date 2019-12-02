/*
**	I P C P E E R
**
**	Copyright 1989, 1990, 1991, 1994, 1995 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		IPC Peer
**	Program		TAGS libipc
**	Project		Text And Graphics System
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Tue Nov 28 15:46:18 MST 1989
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
**
**	Abstract
**
**	This function is part of a low-level interprocess communication
**	package. Higher-level application-specific layers will probably
**	be built on top of this. This is not TAGS specific.
*/

static char copyright[]="Copyright 1989, 1990, 1991, 1994, 1995 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)ipcpeer.c	4.3 96/05/24 jsloan@ncar.ucar.edu";

#include "libipc.h"

/*
**	ipcpeer: creates a socket configured for connectionless
**	datagram IPC.
**
**	Arguments:
**		mbox	number of udp port
**
**	Returns:	socket file descriptor or <0 if error
*/
IPCSOCKET
ipcpeer(IPCMBOX mbox)
	{
	struct sockaddr_in internet;
	IPCSOCKET sock;
	int toggle;

	bzero((char *)&internet,sizeof(internet));
	internet.sin_addr.s_addr=INADDR_ANY;
	internet.sin_family=AF_INET;
	internet.sin_port=mbox;
	if ((sock=socket(AF_INET,SOCK_DGRAM,0))<0)
		return(sock);
	toggle=1;
	if (setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,(char *)&toggle,sizeof(toggle))<0)
		{
		close(sock);
		return(-2);
		}
	if (bind(sock,(struct sockaddr *)&internet,sizeof(internet))<0)
		{
		close(sock);
		return(-3);
		}
	ipcregister(sock);
	return(sock);
	}
