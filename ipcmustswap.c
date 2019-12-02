/*
**	I P C M U S T S W A P
**
**	Copyright 1991, 1992, 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		IPC Must Swap
**	Program		libipc
**	Project		IPC Library
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Thu Aug 15 13:58:27 MDT 1991
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
**
**	Abstract
**
**	This function is part of a low-level interprocess communication
**	package. Higher-level application-specific layers will probably
**	be built on top of this.
*/

static char copyright[]="Copyright 1991, 1992, 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)ipcmustswap.c	4.2 94/05/19 jsloan@ncar.ucar.edu";

#include "libipc.h"

/*
**	ipcmustswap: returns true if the host architecture requires
**	byte swapping. This clever idea was borrowed from X11. In
**	general, you don't need it since you just use htonl(), etc.,
**	but if swapping is expensive, this could be handy.
*/
int
ipcmustswap(void)
	{
	static unsigned long mustswap=1; /* thread safe: read-only */
	return(*(char *)&mustswap);
	}
