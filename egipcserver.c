/*
**	Server portion of multiple processor IPC test. Text received
**	by server from client is displayed on stdout.
**
**	Arguments:	<port number>
**
**	@(#)egipcserver.c	4.2 94/05/19 jsloan@ncar.ucar.edu
*/

#include <stdio.h>
#include "ipc.h"

main(argc,argv)
int argc;
char **argv;
	{
	IPCSOCKET socket, socket2;
	IPCPORT port;
	char buffer[1024];
	int bytes;

	if (argc!=2)
		fprintf(stderr,"usage: %s port\n",argv[0]), exit(1);

	fprintf(stderr,"%s: port=%s\n",argv[0],argv[1]);
	port=ipcport(argv[1]);
	fprintf(stderr,"%s: port=%d\n",argv[0],port);
	if (port==0)
		perror("ipcport"), exit(2);
	socket=ipcserver(port);
	fprintf(stderr,"%s: socket=%d\n",argv[0],socket);
	if (socket<0)
		perror("ipcserver"), exit(3);
	socket2=ipcaccept(socket);
	fprintf(stderr,"%s: socket2=%d\n",argv[0],socket2);
	if (socket2<0)
		perror("ipcaccept"), exit(4);
	while ((bytes=ipcvrecv(socket2,buffer,sizeof(buffer)))>0)
		writes(1,buffer,bytes);
	if (bytes<0)
		{
		fprintf(stderr,"%s: ipcvrecv=%d\n",argv[0],bytes);
		perror("ipcvrecv"), exit(5);
		}
	exit(0);
	}
