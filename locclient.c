/*
**	L O C C L I E N T
**
**	Copyright 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		Local Client
**	Program		libipc
**	Project		IPC Library
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Fri Sep 30 10:13:02 MDT 1994
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
**
**	Abstract
**
**	This function is part of a low-level interprocess communication
**	package. Higher-level application-specific layers will probably
**	be built on top of this.
*/

static char copyright[]="Copyright 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)locclient.c	1.2 94/10/07 jsloan@ncar.ucar.edu";

#include "libipc.h"

/*
**	locclient: request a connection to a local server via a
**	UNIX domain socket in the file system name space.
*/
IPCSOCKET
locclient(char *path)
	{
	struct sockaddr_un sockpath;
	IPCSOCKET sock;
	int length;

	length=strlen(path);
	if (length>sizeof(sockpath.sun_path))
		return(-2);
	if ((sock=socket(AF_UNIX,SOCK_STREAM,0))<0)
		return(sock);
	(void)bzero((char *)&sockpath,sizeof(sockpath));
	sockpath.sun_family=AF_UNIX;
	(void)strncpy(sockpath.sun_path,path,sizeof(sockpath.sun_path));
	length+=sizeof(sockpath)-sizeof(sockpath.sun_path);
	if (connect(sock,(struct sockaddr *)&sockpath,length)<0)
		{
		close(sock);
		return(-3);
		}
	ipcregister(sock);
	return(sock);
	}
