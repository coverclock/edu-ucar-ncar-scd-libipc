/*
**	L O C S E R V E R
**
**	Copyright 1994, 1995 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		Local Server
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

static char copyright[]="Copyright 1994, 1995 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)locserver.c	1.7 96/05/24 jsloan@ncar.ucar.edu";

#include "libipc.h"
#include <unistd.h>

/*
**	locserver: establish a listening socket for connection
**	requests from local clients in the UNIX domain in the
**	file system name space.
*/
IPCSOCKET
locserver(char *path)
	{
	struct sockaddr_un sockpath;
	IPCSOCKET sock;
	int length, toggle;

	length=strlen(path);
	if (length>sizeof(sockpath.sun_path))
		return(-2);
	if ((sock=socket(AF_UNIX,SOCK_STREAM,0))<0)
		return(sock);
	toggle=1;
/*
**	We go ahead and unconditionally delete the old path. We figure
**	if the caller is calling "locserver", it's up to them to make
**	sure they're not conflicting with the same path in another
**	application. This allows locserver to be called successively
**	without having to worry about the housekeeping. The SO_REUSEADDR
**	works on some UNIXen to avoid this, but not under others (e.g.
**	Linux 1.2.13). Note that there is a race condition here in that
**	another locserver could create that path again in between the
**	unlink and the bind. Also, woe to the caller who names a file
**	that is not a UNIX domain socket. It'll be magically transformed
**	into one (with a different inode number) when this completes.
*/
	(void)unlink(path);
/*
	if (setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,(char *)&toggle,sizeof(toggle))<0)
		{
 		(void)close(sock);
		return(-3);
		}
*/
	(void)bzero((char *)&sockpath,sizeof(sockpath));
	sockpath.sun_family=AF_UNIX;
	(void)strncpy(sockpath.sun_path,path,sizeof(sockpath.sun_path));
	length+=sizeof(sockpath)-sizeof(sockpath.sun_path);
	if (bind(sock,(struct sockaddr *)&sockpath,length)<0)
		{
		(void)close(sock);
		return(-4);
		}
	if (listen(sock,SOMAXCONN)<0)
		{
		(void)close(sock);
		return(-5);
		}
	ipcregister(sock);
	return(sock);
	}
