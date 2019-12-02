/*
**	I P C F O R M A T
**
**	Copyright 1990, 1991, 1992, 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		IPC Format
**	Program		libipc
**	Project		IPC Library
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Tue Jun 12 12:29:17 MDT 1990
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
**
**	Abstract
**
**	This function is part of a low-level interprocess communication
**	package. Higher-level application-specific layers will probably
**	be built on top of this.
*/

static char copyright[]="Copyright 1990, 1991, 1992, 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)ipcformat.c	4.1 94/05/16 jsloan@ncar.ucar.edu";

#include "libipc.h"

/*
**	ipcformat: present an internet address and port number in
**	a canonical, readable, ASCII form.
**
**	Arguments:
**		addr	internet address
**		port	port number
**		buffer	buffers in which to place string
**
**	Returns:	pointer to static array holding printable
**			string.
*/
char *
ipcformat(IPCADDRESS addr, IPCPORT port, char *buffer)
	{
	struct in_addr address;

	if (buffer!=NULL)
		{
		address.s_addr=addr;
		(void)sprintf(buffer,"%s.%d",inet_ntoa(address),ntohs(port));
		}

	return(buffer);
	}
