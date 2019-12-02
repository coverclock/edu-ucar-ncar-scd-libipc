/*
**	I P C P U T
**
**	Copyright 1989, 1990, 1991, 1992, 1994, 1995 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		IPC Put
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

static char copyright[]="Copyright 1989, 1990, 1991, 1992, 1994, 1995 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)ipcput.c	4.3 95/08/02 jsloan@ncar.ucar.edu";

#include "libipc.h"

/*
**	ipcput: send a datagram through a connectionless socket.
**
**	Arguments:
**		sock	datagram socket
**		buffer	address of buffer
**		length	length of buffer
**		addr	address to which to send
**		mbox	port to which to send
**
**	Returns:	number of bytes sent, 0 if connection
**			lost, <0 if error
*/
int
ipcput(IPCSOCKET sock, char *buffer, int length, IPCADDRESS addr, IPCMBOX mbox)
	{
	struct sockaddr_in internet;
	int l, n;

	if (buffer==NULL)
		{
		seterrno(EINVAL);
		return(-1);
		}

	bzero((char *)&internet,sizeof(internet));
	internet.sin_addr.s_addr=addr;
	internet.sin_family=AF_INET;
	internet.sin_port=mbox;

	for (n=0; n<length; n+=l)
		if ((l=sendto(sock,buffer+n,length-n,0,(struct sockaddr *)&internet,sizeof(internet)))<=0)
			return(l);

	return(n);
	}
