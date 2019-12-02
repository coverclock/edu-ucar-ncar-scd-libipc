/*
**	I P C C L O S E
**
**	Copyright 1989, 1990, 1991, 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		IPC Close
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

static char copyright[]="Copyright 1989, 1990, 1991, 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)ipcclose.c	4.1 94/05/16 jsloan@ncar.ucar.edu";

#include "libipc.h"

/*
**	ipcclose: shutdown and close a connected socket.
**
**	Arguments:
**		sock	connected socket
**
**	Returns:	0 or <0 if error
**
**	N.B.	Recent discussions in comp.protocols.tcp-ip
**		suggest that the shutdown is rather rude;
**		it may result in data not yet flushed from
**		in-kernel buffers will be lost following a
**		shutdown, which (it is said) corresponds to
**		the TCP ABORT request. We've commented it out
**		here as a precaution.
*/
int
ipcclose(IPCSOCKET sock)
	{
/*****
	if (shutdown(sock,2)<0)
		return(-1);
*****/
	ipcunregister(sock);
	if (close(sock)<0)
		return(-2);
	return(0);
	}
