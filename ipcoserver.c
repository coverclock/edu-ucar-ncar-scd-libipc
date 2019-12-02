/*
**	I P C O S E R V E R
**
**	Copyright 1995 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		IPC Optioned Server
**	Program		libipc
**	Project		libipc
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Mon Jul 24 07:45:39 MDT 1995
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
**
**	Abstract
**
**	This function is part of a low-level interprocess communication
**	package. Higher-level application-specific layers will probably
**	be built on top of this.
*/

static char copyright[]="Copyright 1995 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)ipcoserver.c	1.5 96/05/24 jsloan@ncar.ucar.edu";

#include "libipc.h"
#include <netinet/tcp.h>

/*
**	ipcoserver: establish a bound, unconnected socket on the local
**	host to the specified port, using caller defined optimizations.
**
**	Arguments:
**		port	port number (in binary network byte order,
**			as if from ipcport)
**		shift	the TCP window shift value (RFC1323)
**		sndbuf	the maximum send socket buffer size in bytes
**		rcvbuf	the maximum receive socket buffer size in bytes
**		nodelay	if true, do not delay to coalesce packets
**
**	Returns:	connected stream socket, or <0 if error
*/
IPCSOCKET
ipcoserver(IPCPORT port, int shift, int sndbuf, int rcvbuf, int nodelay)
	{
	struct sockaddr_in internet;
	IPCSOCKET sock;
	int toggle;

	bzero((char *)&internet,sizeof(internet));
	internet.sin_addr.s_addr=INADDR_ANY;
	internet.sin_family=AF_INET;
	internet.sin_port=port;
	if ((sock=socket(AF_INET,SOCK_STREAM,0))<0)
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

#ifdef TCP_WINSHIFT
	/*
	** If shift==(-1) window shifting is disabled.
	*/
	if (setsockopt(sock,IPPROTO_TCP,TCP_WINSHIFT,(char *)&shift,sizeof(shift))<0)
		{
		close(sock);
		return(-5);
		}
#endif /* TCP_WINSHIFT */

#ifdef TCP_RFC1323
	if (shift>0)
		if (setsockopt(sock,IPPROTO_TCP,TCP_RFC1323,(char *)&shift,sizeof(shift))<0)
			{
			close(sock);
			return(-4);
			}
#endif /* TCP_RFC1323 */

#ifdef TCP_NODELAY
	if (nodelay>0)
		if (setsockopt(sock,IPPROTO_TCP,TCP_NODELAY,(char *)&nodelay,sizeof(nodelay))<0)
			{
			close(sock);
			return(-6);
			}
#endif /* TCP_NODELAY */

#ifdef SO_SNDBUF
	if (sndbuf>0)
		if (setsockopt(sock,SOL_SOCKET,SO_SNDBUF,(char *)&sndbuf,sizeof(sndbuf))<0)
			{
			close(sock);
			return(-7);
			}
#endif /* SO_SNDBUF */

#ifdef SO_RCVBUF
	if (rcvbuf>0)
		if (setsockopt(sock,SOL_SOCKET,SO_RCVBUF,(char *)&rcvbuf,sizeof(rcvbuf))<0)
			{
			close(sock);
			return(-8);
			}
#endif /* SO_RCVBUF */

	if (listen(sock,SOMAXCONN)<0)
		{
		close(sock);
		return(-9);
		}

	ipcregister(sock);

	return(sock);
	}
