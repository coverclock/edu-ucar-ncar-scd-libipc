/*
**	Client portion of multiple processor IPC test. Text read
**	by client on stdin is transmitted to server.
**
**	Arguments:	<host name> <port number>
**
**	@(#)egipcclient.c	4.2 94/05/19 jsloan@ncar.ucar.edu
*/

#include <stdio.h>
#include "errors.h"
#include "ipc.h"

main(argc,argv)
int argc;
char **argv;
	{
	IPCSOCKET socket;
	IPCADDRESS address;
	IPCPORT port;
	char buffer[1024];
	int bytes, sent;

	if (argc!=3)
		fprintf(stderr,"usage: %s host port\n",argv[0]), exit(1);

	fprintf(stderr,"%s: host=%s, port=%s\n",argv[0],argv[1],argv[2]);
	address=ipcaddress(argv[1]);
	fprintf(stderr,"%s: host=%#lx\n",argv[0],address);
	if (address==0)
		perror("ipcaddress"), exit(2);
	port=ipcport(argv[2]);
	fprintf(stderr,"%s: port=%d\n",argv[0],port);
	if (port==0)
		perror("ipcport"), exit(3);
	socket=ipcclient(address,port);
	fprintf(stderr,"%s: socket=%d\n",argv[0],socket);
	if (socket<0)
		perror("ipcclient"), exit(4);
	while ((bytes=read(0,buffer,sizeof(buffer)))>0)
		if ((sent=ipcsend(socket,buffer,bytes))!=bytes)
			{
			fprintf(stderr,"%s: bytes=%d, sent=%d\n",
				argv[0],bytes,sent);
			perror("ipcsend"), exit(5);
			}
	exit(0);
	}
