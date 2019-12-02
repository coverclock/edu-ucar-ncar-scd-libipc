/*
**	I P C P A I R
**
**	Copyright 1989, 1990, 1991, 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		IPC Pair
**	Program		TAGS libipc
**	Project		Text And Graphics System
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Wed Oct 25 16:09:17 MDT 1989
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
**
**	Abstract
**
**	This function is part of a low-level interprocess communication
**	package. Higher-level application-specific layers will probably
**	be built on top of this. This is not TAGS specific.
*/

static char copyright[]="Copyright 1989, 1990, 1991, 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)ipcpair.c	4.1 94/05/16 jsloan@ncar.ucar.edu";

#include "libipc.h"

/*
**	ipcpair: creates a connected pair of sockets. The socket that
**	is returned as a result is "registered" so that it may work
**	with ipcready/ipctimed(), while the one returned in the
**	value/result parameter is not. The intent here is that a
**	process may create such a pair, then fork off a child. One process
**	would always use a blocking ipcrecv, while the other would
**	have the option of multiplexing with ipcready/ipctimed etc.
**
**	A parent who would fork off many children would leave the
**	unregistered sockets to them, while multiplexing all the
**	registered sockets.
**
**	Argument:
**		sockp	address of a socket variable into which
**			is placed the unregistered socket number if
**			the function is successful.
**
**	Returns:	new socket or <0 if error
*/
IPCSOCKET
ipcpair(IPCSOCKET *sockp)
	{
	IPCSOCKET pair[2];

	if (socketpair(AF_UNIX,SOCK_STREAM,0,pair)<0)
		return(-1);
	*sockp=pair[1];
	ipcregister(pair[0]);
	return(pair[0]);
	}
