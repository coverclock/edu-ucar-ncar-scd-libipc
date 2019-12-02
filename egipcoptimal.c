/*
**	E G I P C O P T I M A L
**
**	Copyright 1995 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		Example IPC Optimal
**	Program		egipcoptimal
**	Project		libipc
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Fri Aug  4 13:53:27 MDT 1995
**	Organization	NCAR/SCD, P.O. Box 3000, Boulder CO 80307
**
**	Abstract
**
**
*/

static char copyright[]="Copyright 1995 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)egipcoptimal.c	1.1 95/08/04 jsloan@ncar.ucar.edu";

#include <stdio.h>
#include <string.h>
#include "ipc.h"

#define USAGE	"[ -b mb/s ] [ -l ms ]"

void
main(int argc, char **argv)
	{
	int opt;
	extern char *optarg;
	extern int optind;
	int inerror, usage;
	FILE *fp;
	char *argvzero;

	int bandwidth, latency, shift, bufsiz, product;

	/*
	** Crack the command line and pick out the options.
	*/

	argvzero=((argvzero=strrchr(argv[0],'/'))==NULL)?argv[0]:argvzero+1;
	usage=0;
	bandwidth=0;
	latency=0;
	while ((opt=getopt(argc,argv,"b:l:"))>=0)
		{
		inerror=0;
		switch (opt)
			{
		case 'b':
			bandwidth=atoi(optarg);
			break;
		case 'l':
			latency=atoi(optarg);
			break;
		default:
			usage++;
			}
		if (inerror>0)
			{
			fprintf(stderr,"%s: bad value -- -%c %s\n",
				argvzero,opt,optarg);
			usage++;
			}
		}
	if (usage)
		{
		fprintf(stderr,"usage: %s %s\n",argvzero,USAGE);
		exit(1);
		}

	product=ipcproduct(bandwidth,latency);
	shift=ipcoptimal(product,&bufsiz);

	printf("%dMb/s*%dms=%dbytes shift=%d buffer=%dbytes\n",
		bandwidth,latency,product,shift,bufsiz);

	exit(0);
	}
