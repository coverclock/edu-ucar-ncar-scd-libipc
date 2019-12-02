/*
**	Server portion of multiple processor IPC test. Text received
**	by server from client is displayed on stdout.
**
**	Arguments:	<port number>
**
**	@(#)egdgmserver.c	4.2 94/05/19 jsloan@ncar.ucar.edu
*/

#include <stdio.h>
#include "nmalloc.h"
#include "ipc.h"

main(argc,argv)
int argc;
char **argv;
	{
	IPCSOCKET socket;
	IPCADDRESS faddress;
	IPCPORT port, fport;
	char buffer[1024], address[IPCADDRESSLEN];
	int bytes;

	if (argc!=2)
		fprintf(stderr,"usage: %s port\n",argv[0]), exit(1);

	fprintf(stderr,"%s: port=%s\n",argv[0],argv[1]);

	port=ipcmbox(argv[1]);
	fprintf(stderr,"%s: port=%d\n",argv[0],port);
	if (port==0)
		perror("ipcmbox"), exit(2);

	socket=ipcpeer(port);
	fprintf(stderr,"%s: socket=%d\n",argv[0],socket);
	if (socket<0)
		perror("ipcpeer"), exit(3);

	while ((bytes=ipcget(socket,buffer,sizeof(buffer),&faddress,&fport))>0)
		printf("%s: %*.*s\n",
			ipcformat(faddress,fport,address),bytes,bytes,buffer);

	if (bytes<0)
		{
		fprintf(stderr,"%s: ipcget=%d\n",argv[0],bytes);
		perror("ipcget"), exit(5);
		}

	bufpool(stderr);
	kvpool(stderr);
	nmallocinfo(stderr);
	nmallocpool(stderr);
	ipcdump(stderr);
	
	exit(0);
	}
