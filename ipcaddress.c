/*
**	I P C A D D R E S S
**
**	Copyright 1989, 1990, 1991, 1992, 1994, 1995 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		IPC Address
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
static char sccsid[]="@(#)ipcaddress.c	4.7 96/06/11 jsloan@ncar.ucar.edu";

#include "libipc.h"

/*
**	ipcaddress: given a string containing a host name or internet
**	address in standard dot notation, lookup the primary internet
**	address.
**
**	ipcaddresses: same as ipcaddress but return the ndx-th
**	alternate address (the primary is ndx 0).
**
**	Arguments:
**		name	string containing host name or internet
**			address in dot notation
**		ndx	index into address table.
**
**	Returns:	internet address or 0 if error
**
**	N.B.	Some of the machinations here are due to UNICOS
**		and how it handles 32bit Internet addresses in
**		a 64bit CRAY longword. This code has been tested
**		on (at least) a 32bit Sun-4 and a 64bit CRAY YMPEL.
*/
IPCADDRESS
ipcaddresses(char *name, int ndx)
	{
	struct hostent *h;
	IPCADDRESS address;
	struct in_addr inaddr;
	int tndx;

	if (name==NULL)
		{
		seterrno(EINVAL);
		return((IPCADDRESS)0);
		}

	address=(IPCADDRESS)0;

	P(sem_ipcaddress_libi);

	if ((h=gethostbyname(name))!=NULL)
		{
		if (h->h_addrtype==AF_INET)
			{
			for (tndx=0; tndx<ndx; tndx++)
				if (h->h_addr_list[tndx]==NULL)
					break;
			if (h->h_addr_list[tndx]==NULL)
				seterrno(EADDRNOTAVAIL);
			else
				{
				bzero((char *)&inaddr,sizeof(inaddr));
				bcopy((char *)h->h_addr_list[tndx],
					(char *)&inaddr,
					h->h_length<sizeof(inaddr)?
						h->h_length:sizeof(inaddr));
				address=inaddr.s_addr;
				}
			}
		else
			seterrno(EAFNOSUPPORT);
		}
	else if ((inaddr.s_addr=(IPCADDRESS)inet_addr(name))==(-1))
		seterrno(EADDRNOTAVAIL);
	else
		address=(IPCADDRESS)inaddr.s_addr;

	V(sem_ipcaddress_libi);

	return(address);
	}

IPCADDRESS
ipcaddress(char *name)
	{
	return(ipcaddresses(name,0));
	}
