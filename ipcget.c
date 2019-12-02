/*
**	I P C G E T
**
**	Copyright 1989, 1990, 1991, 1992, 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		IPC Get
**	Program		libipc
**	Project		IPC Library
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Tue Nov 28 15:46:52 MST 1989
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
**
**	Abstract
**
**	This function is part of a low-level interprocess communication
**	package. Higher-level application-specific layers will probably
**	be built on top of this.
*/

static char copyright[]="Copyright 1989, 1990, 1991, 1992, 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)ipcget.c	4.1 94/05/16 jsloan@ncar.ucar.edu";

#include "libipc.h"

/*
**	ipcget: receive a datagram from a connectionless socket.
**
**	Arguments:
**		sock	datagram socket
**		buffer	address of buffer
**		length	length of buffer
**		addrp	address of address variable into which will be
**			placed the address of the sender
**		mboxp	address of port variable into which will be
**			placed the port of the sender
**
**	Returns:	number of bytes received, 0 if connection
**			lost, <0 if error
*/
int
ipcget(IPCSOCKET sock, char *buffer, int length, IPCADDRESS *addrp, IPCMBOX *mboxp)
	{
	struct sockaddr_in internet;
	int n, size;

	if (buffer==NULL)
		{
		seterrno(EINVAL);
		return(-1);
		}

	size=sizeof(internet);
	bzero((char *)&internet,size);
	n=recvfrom(sock,buffer,length,0,(struct sockaddr *)&internet,&size);

	*addrp=internet.sin_addr.s_addr;
	*mboxp=internet.sin_port;

	return(n);
	}
