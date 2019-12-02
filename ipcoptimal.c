/*
**	I P C O P T I M A L
**
**	Copyright 1995 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		IPC Optimal
**	Program		libipc
**	Project		libipc
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Wed Aug  2 10:19:51 MDT 1995
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
**
**	Abstract
**
**	This function is part of a low-level interprocess communication
**	package. Higher-level application-specific layers will probably
**	be built on top of this.
*/

static char copyright[]="Copyright 1995 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)ipcoptimal.c	1.3 95/08/02 jsloan@ncar.ucar.edu";

#include "libipc.h"

#define BIAS	((int)0xffff)

/*
**	ipcproduct: calculate the bandwidth*delay product in bytes
**	given a bandwidth in megabits per second (mb/s) and a delay
**	in milliseconds (ms). N.B.: megaBITS, MILLIseconds. These
**	units were chosen because there are the units in which
**	these metrics are typically measured.
*/
int
ipcproduct(int mbs, int ms)
	{
	int result;
	double product;

	product=(((double)mbs/8.0)*1000000.0)*((double)ms/1000.0);
	result=product+0.5;
	return(result);
	}

/*
**	ipcoptimal: calculate the optimal TCP window shift and
**	buffer size in bytes given a bandwidth*delay product in
**	bytes. The shift value is returned as a value of the
**	function.
*/
int
ipcoptimal(int product, int *bufsizp)
	{
	int shift, try, bufsiz, result, base, factor;

	bufsiz=(product+BIAS-1)/BIAS;
	shift=0;
	try=1;
	while (try<bufsiz)
		{
		shift++;
		try<<=1;
		}
	base=product/try;
	factor=((base+try-1)/try)*try;
	result=factor*try;

	if (bufsizp!=NULL)
		*bufsizp=result;

	return(shift);
	}
