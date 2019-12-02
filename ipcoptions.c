/*
**	I P C O P T I O N S
**
**	Copyright 1991, 1992, 1994, 1995 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		IPC Options
**	Program		TAGS libipc
**	Project		Text And Graphics System
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Wed Jul 10 10:40:14 MDT 1991
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
**
**	Abstract
**
**	This function is part of a low-level interprocess communication
**	package. Higher-level application-specific layers will probably
**	be built on top of this. This is not TAGS specific.
*/

static char copyright[]="Copyright 1991, 1992, 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)ipcoptions.c	4.4 96/05/24 jsloan@ncar.ucar.edu";

#include "libipc.h"
#include <netinet/tcp.h>

/*
**	ipcnonblocking: prevents the specified socket from blocking
**	on an operation. An error is returned and errno is returned
**	as EWOULDBLOCK instead.
**
**	ipcblocking: allows the specified socket to block on an
**	operation.
**
**	Arguments:
**		sock	connected socket
**
**	Returns:	0 if copacetic, <0 otherwise
*/
int
ipcnonblocking(IPCSOCKET sock)
	{
	int flags;

	if ((flags=fcntl(sock,F_GETFL,0))<0)
		return(flags);
	return(fcntl(sock,F_SETFL,(flags|FNDELAY)));
	}

int
ipcblocking(sock)
IPCSOCKET sock;
	{
	int flags;

	if ((flags=fcntl(sock,F_GETFL,0))<0)
		return(flags);
	return(fcntl(sock,F_SETFL,(flags&(~FNDELAY))));
	}

/*
**	ipcparanoid: activate the Keep Alive timer on a socket so
**	the process is informed asynchronously via SIGPIPE if its
**	IPC partner goes away.
**
**	Arguments:
**		sock	connected socket
**
**	Returns:	>=0 if okay
**			<0 if error
*/
int
ipcparanoid(IPCSOCKET sock)
	{
	int toggle;

	toggle=1;
	return(setsockopt(sock,SOL_SOCKET,SO_KEEPALIVE,(char *)&toggle,sizeof(toggle)));
	}

/*
**	ipcdebug: turn socket level debugging on.
**
**	ipcnodebug: turn socket level debugging off.
**
**	Arguments:
**		sock	connected socket
**
**	Returns:	>=0 if okay
**			<0 if error
*/
int
ipcdebug(IPCSOCKET sock)
	{
	int toggle;

	toggle=1;
	return(setsockopt(sock,SOL_SOCKET,SO_DEBUG,(char *)&toggle,sizeof(toggle)));
	}

int
ipcnodebug(IPCSOCKET sock)
	{
	int toggle;

	toggle=0;
	return(setsockopt(sock,SOL_SOCKET,SO_DEBUG,(char *)&toggle,sizeof(toggle)));
	}

/*
**	ipclinger: when an ipcclose is performed, wait until any unsent
**	data is sent.
**
**	ipcnolinger: when an ipcclose is performed, return as quickly
**	as possible leaving unsent data to be sent afterwards.
**
**	Arguments:
**		sock	connected socket
**
**	Returns:	>=0 if okay
**			<0 if error
*/
int
ipclinger(IPCSOCKET sock)
	{
	struct linger delay;

	delay.l_onoff=1;
	delay.l_linger=1;
	return(setsockopt(sock,SOL_SOCKET,SO_LINGER,(char *)&delay,sizeof(delay)));
	}

int
ipcnolinger(IPCSOCKET sock)
	{
	struct linger delay;

	delay.l_onoff=0;
	delay.l_linger=0;
	return(setsockopt(sock,SOL_SOCKET,SO_LINGER,(char *)&delay,sizeof(delay)));
	}

/*
**	ipcotions: return socket parameters.
**
**	vector[0] <-	maximum segment size
**	vector[1] <-	send buffer size
**	vector[2] <-	receive buffer size
**	vector[3] <-	debug flag
**	vector[4] <-	keepalive flag
**	vector[5] <-	linger flag
**	vector[6] <-	nodelay flag
**	vector[7] <-	rfc1323 flag
**	vector[8] <-	window shift flag
**	vector[9] <-	send window shift
**	vector[10] <-	receive window shift
**
**	For flags:	0 means feature not enabled.
**			-1 means getsockopt failed.
**			-2 means feature not available.
*/
int
ipcoptions(IPCSOCKET sock, int *vector, int *vsize)
	{
	int cmaxseg, smaxseg, cwinshift[3], swinshift, cnodelay, snodelay;
	int csndbuf, ssndbuf, crcvbuf, srcvbuf, crfc1323, srfc1323;
	int ckeepalive, skeepalive, cdebug, sdebug, clinger, slinger;
	int vndx, vmax;

#ifdef TCP_MAXSEG
	smaxseg=sizeof(cmaxseg);
	if (getsockopt(sock,IPPROTO_TCP,TCP_MAXSEG,(char *)&cmaxseg,&smaxseg)<0)
		{
		perror("getsockopt TCP_MAXSEG");
		cmaxseg=(-1);
		}
#else /* TCP_MAXSEG */
	cmaxseg=(-2);
	smaxseg=sizeof(cmaxseg);
#endif /* TCP_MAXSEG */

#ifdef TCP_WINSHIFT
	swinshift=sizeof(cwinshift);
	if (getsockopt(sock,IPPROTO_TCP,TCP_WINSHIFT,(char *)&cwinshift,&swinshift)<0)
		{
		perror("getsockopt TCP_WINSHIFT");
		cwinshift[0]=(-1);
		cwinshift[1]=(-1);
		cwinshift[2]=(-1);
		}
#else /* TCP_WINSHIFT */
	cwinshift[0]=(-2);
	cwinshift[1]=(-2);
	cwinshift[2]=(-2);
	swinshift=sizeof(cwinshift);
#endif /* TCP_WINSHIFT */

#ifdef TCP_RFC1323
	srfc1323=sizeof(crfc1323);
	if (getsockopt(sock,IPPROTO_TCP,TCP_RFC1323,(char *)&crfc1323,&srfc1323)<0)
		{
		perror("getsockopt TCP_RFC1323");
		crfc1323=(-1);
		}
#else /* TCP_RFC1323 */
	crfc1323=(-2);
	srfc1323=sizeof(crfc1323);
#endif /* TCP_RFC1323 */

#ifdef TCP_NODELAY
	snodelay=sizeof(cnodelay);
	if (getsockopt(sock,IPPROTO_TCP,TCP_NODELAY,(char *)&cnodelay,&snodelay)<0)
		{
		perror("getsockopt TCP_NODELAY");
		cnodelay=(-1);
		}
#else /* TCP_NODELAY */
	cnodelay=(-2);
	snodelay=sizeof(cnodelay);
#endif /* TCP_NODELAY */

#ifdef SO_SNDBUF
	ssndbuf=sizeof(csndbuf);
	if (getsockopt(sock,SOL_SOCKET,SO_SNDBUF,(char *)&csndbuf,&ssndbuf)<0)
		{
		perror("getsockopt SO_SNDBUF");
		csndbuf=(-1);
		}
#else /* SO_SNDBUF */
	csndbuf=(-2);
	ssndbuf=sizeof(csndbuf);
#endif /* SO_SNDBUF */

#ifdef SO_RCVBUF
	srcvbuf=sizeof(crcvbuf);
	if (getsockopt(sock,SOL_SOCKET,SO_RCVBUF,(char *)&crcvbuf,&srcvbuf)<0)
		{
		perror("getsockopt SO_RCVBUF");
		crcvbuf=(-1);
		}
#else /* SO_RCVBUF */
	crcvbuf=(-2);
	srcvbuf=sizeof(crcvbuf);
#endif /* SO_RCVBUF */

#ifdef SO_KEEPALIVE
	skeepalive=sizeof(ckeepalive);
	if (getsockopt(sock,SOL_SOCKET,SO_KEEPALIVE,(char *)&ckeepalive,&skeepalive)<0)
		{
		perror("getsockopt SO_KEEPALIVE");
		ckeepalive=(-1);
		}
#else /* SO_KEEPALIVE */
	ckeepalive=(-2);
	skeepalive=sizeof(ckeepalive);
#endif /* SO_KEEPALIVE */

#ifdef SO_DEBUG
	sdebug=sizeof(cdebug);
	if (getsockopt(sock,SOL_SOCKET,SO_DEBUG,(char *)&cdebug,&sdebug)<0)
		{
		perror("getsockopt SO_DEBUG");
		cdebug=(-1);
		}
#else /* SO_DEBUG */
	cdebug=(-2);
	sdebug=sizeof(cdebug);
#endif /* SO_DEBUG */

#ifdef SO_LINGER
	slinger=sizeof(clinger);
	if (getsockopt(sock,SOL_SOCKET,SO_LINGER,(char *)&clinger,&slinger)<0)
		{
		perror("getsockopt TCP_RCVBUF");
		clinger=(-1);
		}
#else /* SO_LINGER */
	clinger=(-2);
	slinger=sizeof(clinger);
#endif /* SO_LINGER */

	vndx=0;
	vmax=(*vsize)/sizeof(int);
	if (vndx<vmax) vector[vndx++]=cmaxseg;
	if (vndx<vmax) vector[vndx++]=csndbuf;
	if (vndx<vmax) vector[vndx++]=crcvbuf;
	if (vndx<vmax) vector[vndx++]=cdebug;
	if (vndx<vmax) vector[vndx++]=ckeepalive;
	if (vndx<vmax) vector[vndx++]=clinger;
	if (vndx<vmax) vector[vndx++]=cnodelay;
	if (vndx<vmax) vector[vndx++]=crfc1323;
	if (vndx<vmax) vector[vndx++]=cwinshift[0];
	if (vndx<vmax) vector[vndx++]=cwinshift[1];
	if (vndx<vmax) vector[vndx++]=cwinshift[2];
	*vsize=vndx*sizeof(int);

	return(0);
	}
