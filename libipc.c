/*
**	L I B I P C
**
**	Copyright 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		Libipc Initialization
**	Program		libipc
**	Project		IPC Library
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Tue May 17 16:15:35 MDT 1994
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
*/

static char copyright[]="Copyright 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)libipc.c	1.5 94/06/01 jsloan@ncar.ucar.edu";

#include "libipc.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#ifdef PTHREADS_LIBTOOLS

#define SEMAPHORES	(3)

static SEMAPHORE semaphores[SEMAPHORES];

const SEMAPHORE *sem_ipcready_libi=&semaphores[0];
const SEMAPHORE *sem_ipcaddress_libi=&semaphores[1];
const SEMAPHORE *sem_ipcport_libi=&semaphores[2];

static char *names[]=
	{
	"sem_ipcready_libi",
	"sem_ipcaddress_libi",
	"sem_ipcport_libi"
	};

void
libipcdump(FILE *fp)
	{
	int i;

	for (i=0; i<SEMAPHORES; i++)
		{
		fputs(names[i],fp);
		fputs(": ",fp);
		semDump(&semaphores[i],fp);
		}
	}

static void
initipc(void)
	{
	int i;

	for (i=0; i<SEMAPHORES; i++)
		semInit(&semaphores[i],1);
	}

void
libipc(void)
	{
	int rc;
	static pthread_once_t once=pthread_once_init;

	rc=pthread_once(&once,initipc);
	assert(rc>=0);
	}

#endif /* PTHREADS_LIBTOOLS */
