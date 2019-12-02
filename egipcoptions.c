/*
**	Tests some of the socket options calls.
**
**	@(#)egipcoptions.c	4.3 95/07/26 jsloan@ncar.ucar.edu
*/

#include <stdio.h>
#include "errors.h"
#include "ipc.h"

main()
	{
	int was, is, now, then, sock;

	if ((sock=ipcserver(20000))<0)
		perror("ipcserver"), exit(1);

	if (ipcparanoid(sock)<0)
		perror("ipcparanoid"), exit(2);

	if (ipclinger(sock)<0)
		perror("ipclinger"), exit(9);

	if (ipcnolinger(sock)<0)
		perror("ipcnolinger"), exit(10);

	if (ipcdebug(sock)<0)
		perror("ipcdebug"), exit(11);

	if (ipcnodebug(sock)<0)
		perror("ipcnodebug"), exit(12);

	if (ipcnonblocking(sock)<0)
		perror("ipcnoblocking"), exit(13);

	if (ipcblocking(sock)<0)
		perror("ipcblocking"), exit(14);

	}
