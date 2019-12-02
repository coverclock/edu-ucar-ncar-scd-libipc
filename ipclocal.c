/*
**	I P C L O C A L
**
**	Copyright 1989, 1990, 1991, 1992, 1994, 1995 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		IPC Local
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
static char sccsid[]="@(#)ipclocal.c	4.3 95/08/06 jsloan@ncar.ucar.edu";

#include "libipc.h"

/*
**	ipclocal: returns true if the supplied internet address
**	references the local machine. The address is compared to
**	the internet address of the local machine as well as to the
**	internet address of the pseudo-maching "localhost".
**
**	Arguments:
**		address	internet address in binary (as if from
**			ipcaddress)
**
**	Returns:	0 if non-local, 1 if local, <0 if error
*/
int
ipclocal(IPCADDRESS address)
	{
	char name[IPCHOSTNAMELEN];
	IPCADDRESS host;
	int ndx;

	if (address==0x7f000001L)
		return(1);

	if ((host=ipcaddress(IPCLOCALHOST))!=0L)
		if (address==host)
			return(1);

	if (ipchost(name)!=NULL)
		for (ndx=0; ; ndx++)
			if ((host=ipcaddresses(name,ndx))==0L)
				break;
			else if (address==host)
				return(1);

	return(0);
	}
