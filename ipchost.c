/*
**	I P C H O S T
**
**	Copyright 1990, 1991, 1992, 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		IPC Host
**	Program		libipc
**	Project		IPC Library
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Tue Nov 27 09:22:32 MST 1990
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
**
**	Abstract
**
**	This function is part of a low-level interprocess communication
**	package. Higher-level application-specific layers will probably
**	be built on top of this.
*/

static char copyright[]="Copyright 1990, 1991, 1992, 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)ipchost.c	4.1 94/05/16 jsloan@ncar.ucar.edu";

#include "libipc.h"

/*
**	ipchost: returns the name of the local host.
**
**	Arguments:
**		name	buffer in which name is placed
**
**	Returns:	string containing the name of the local host.
*/
char *
ipchost(char name[IPCHOSTNAMELEN])
	{
	if (gethostname(name,IPCHOSTNAMELEN)<0)
		return(NULL);

	return(name);
	}
