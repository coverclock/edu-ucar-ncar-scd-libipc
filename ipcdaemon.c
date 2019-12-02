/*
**	I P C D A E M O N
**
**	Copyright 1989, 1990, 1991, 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		IPC Daemon
**	Program		TAGS libipc
**	Project		Text And Graphics System
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Fri Oct  6 12:54:48 MDT 1989
**	Organization	NCAR, P.O. Box 3000, Boulder CO 80307
**
**	Abstract
**
**	Turns the calling process into a daemon. There is no rite
**	of exorcism to recover from this, so a call to ipcdaemon() should
**	be commented out while debugging.
**
**	N.B.	Programs which run as children of inetd(8) are not true
**		daemons; this function is NOT appropriate to them
**		(especially since it will close the inetd socket which
**		is file descriptor 0). Also note that inetd(8) is the
**		PREFERRED method of implementing servers, whenever
**		possible.
**
**	BTW, the original version of this function did virtually NO
**	error checking. The second version was ultraparanoid. This
**	one tries to strike a balance somewhere in between. The value
**	returned is zero if everything is copacetic, a negative number
**	if something went wrong (the number indicates the last error
**	detected... the function attempts to continue if possible).
*/

static char copyright[]="Copyright 1989, 1990, 1991, 1994 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)ipcdaemon.c	4.2 94/05/19 jsloan@ncar.ucar.edu";

#include "libipc.h"
#include <stdio.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <fcntl.h>

int
ipcdaemon(void)
	{
	int pid, fd, rc;
	struct sigaction action;

	rc=0;

	/*
	**	Flush stdout and stderr so that any pending output
	**	is displayed prior to the fork.
	*/

	(void)fflush(stdout);
	(void)fflush(stderr);

	/*
	**	Divorce our parent.
	*/

	if ((pid=fork())==(-1))
		return(-1);
	if (pid>0)
		exit(0);

	/*
	**	Close every file descriptor we can lay our hands on.
	**	Ignore errors due to closing closed or unused
	**	descriptors.
	*/

	for (fd=getdtablesize()-1; fd>=0; fd--)
		(void)close(fd);

	/*
	**	Reassign stdin, stdout, stderr to oblivion. The only
	**	way to log errors now is via syslog.
	*/

	if ((fd=open("/",O_RDONLY))==(-1))
		return(-2);

	if (fd!=0)
		{
		if (dup2(fd,0)<0)
			rc=(-3);
		else
			{
			if (dup2(0,1)<0)
				rc=(-4);
			if (dup2(0,2)<0)
				rc=(-5);
			}
		(void)close(fd);
		}
	else
		{
		if (dup2(0,1)<0)
			rc=(-6);
		if (dup2(0,2)<0)
			rc=(-7);
		}

	/*
	**	Ditch the controlling terminal association.
	*/

#ifdef TIOCNOTTY
	if ((fd=open("/dev/tty",O_RDWR))==(-1))
		rc=(-8);
	else
		{
		if (ioctl(fd,TIOCNOTTY,(char *)0)==(-1))
			rc=(-9);
		(void)close(fd);
		}
#endif /* TIOCNOTTY */

	/*
	**	Dissociate ourself from the process group.
	*/

	if (setpgrp(0,0)==(-1))
		rc=(-10);

	/*
	**	Ignore job control tty signals.
	*/

        (void)bzero((char *)&action,sizeof(action));
        action.sa_handler=SIG_IGN;

#ifdef SIGTSTP
	if (sigaction(SIGTSTP,&action,NULL)==(-1))
		rc=(-11);
#endif /* SIGTSTP */

#ifdef SIGTTIN
	if (sigaction(SIGTTIN,&action,NULL)==(-1))
		rc=(-12);
#endif /* SIGTTIN */

#ifdef SIGTTOU
	if (sigaction(SIGTTOU,&action,NULL)==(-1))
		rc=(-13);
#endif /* SIGTTOU */

	/*
	**	No one can find us now! Hahahahahahahaha!
	*/

	return(rc);
	}
