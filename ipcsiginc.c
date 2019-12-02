/*
**	I P C S I G I N C
**
**	Copyright 1990, 1991, 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		IPC Signal Counter Increment
**	Program		TAGS libipc
**	Project		Text And Graphics System
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Tue Feb 13 11:08:39 MST 1990
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
**
**	Abstract
**
**	This function is part of a low-level interprocess communication
**	package. Higher-level application-specific layers will probably
**	be built on top of this. This is not TAGS specific.
*/

static char copyright[]="Copyright 1990, 1991, 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)ipcsiginc.c	4.1 94/05/16 jsloan@ncar.ucar.edu";

#include "libipc.h"

/*
**	ipcsiginc: increments a signal counter inside a critical
**	section in which the pertinent signal is blocked.
**	Returns 1 if the previous value of the counter was >0,
**	0 otherwise.
*/
int
ipcsiginc(int sig, int *ctrp)
	{
	int mask, rc;

	mask=0;
	if (sig>0)
		mask=sigblock(sigmask(sig));
	rc=((*ctrp)>0);
	(*ctrp)++;
	if (sig>0)
		(void)sigsetmask(mask);
	return(rc);
	}
