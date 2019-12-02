/*
**	I P C P O R T
**
**	Copyright 1989, 1990, 1991, 1992, 1994, 1995 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		IPC Port
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

static char copyright[]="Copyright 1989, 1990, 1991, 1992, 1994, 1995 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)ipcport.c	4.3 95/07/26 jsloan@ncar.ucar.edu";

#include "libipc.h"
#include "s2i.h"

/*
**	ipcport: return the "tcp" port number corresponding to the
**	string containing the service name or port number. This is
**	used for stream connections.
**
**	ipcmbox: return the "udp" port number corresponding to
**	the string containing the service name or port number.
**	This is used for datagram connections.
**
**	Arguments:
**		service	name of service or port number
**
**	Returns:	port number or 0 if error
*/
static IPCPORT
ipcgenport(char *service, char *protocol)
	{
	struct servent *p, port;
	IPCPORT portnumber;
	int number;

	if (service==NULL)
		{
		seterrno(EINVAL);
		return(0);
		}

	P(sem_ipcport_libi);
	if ((p=getservbyname(service,protocol))!=NULL)
		bcopy((char *)p,(char *)&port,sizeof(port));
	V(sem_ipcport_libi);

	if (p!=NULL)
		portnumber=port.s_port;
	else if (s2i(service,&number)!=1)
		{
		seterrno(EINVAL);
		return(0);
		}
	else
		portnumber=htons((short)number);
	return(portnumber);
	}

IPCPORT
ipcport(char *service)
	{
	return(ipcgenport(service,"tcp"));
	}

IPCPORT
ipcmbox(char *service)
	{
	return(ipcgenport(service,"udp"));
	}
