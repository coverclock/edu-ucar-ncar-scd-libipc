/*
**	Test for kernel buffering of stream sockets
**
**	Arguments:	<port number> <seconds> <bytes>
**
**	@(#)egstreams4.c	4.2 94/05/19 jsloan@ncar.ucar.edu
*/

#include <stdio.h>
#include "libttypes.h"
#include "nmalloc.h"
#include <sys/wait.h>
#include "ipc.h"

#define NETSTAT		"sh -c 'netstat -m|head -13' 2>&1"

main(argc,argv)
int argc;
char **argv;
	{
	int pid, length, total, seconds, bytes;
	IPCSOCKET mainsock, sock;
	char *name, *port, *buffer;

	if (argc<4)
		{
		fprintf(stderr,"usage: %s port seconds bytes\n",argv[0]);
		exit(1);
		}

	name=argv[0];
	port=argv[1];
	seconds=atoi(argv[2]);
	bytes=atoi(argv[3]);
	fprintf(stderr,"%s: %s %d %d\n",name,port,seconds,bytes);

	if ((buffer=namalloc(bytes))==NULL)
		perror("namalloc"), exit(2);

	pid=fork();
	if (pid>0)
		{

		mainsock=ipcserver(ipcport(port));
		fprintf(stderr,"%s: ipcserver(%s)=%d\n",name,port,mainsock);
		if (mainsock<0)
			perror("ipcserver"), exit(3);

		if ((sock=ipcready())!=mainsock)
			perror("ipcready"), exit(4);

		sock=ipcaccept(mainsock);
		fprintf(stderr,"%s: ipcaccept(%d)=%d\n",name,mainsock,sock);
		if (sock<0)
			perror("ipcaccept"), exit(5);

		if (seconds>0)
			sleep(seconds);

		total=0;
		while ((length=ipcvrecv(sock,buffer,bytes))>0)
			{
			total+=length;
			fprintf(stderr,"%s: ipcvrecv=%d total=%d\n",
				name,length,total);
			}
		if (length<0)
			perror("ipcvrecv"), exit(6);

		wait(NULL);

		system(NETSTAT);

		bufpool(stderr);
		kvpool(stderr);
		nmallocinfo(stderr);
		nmallocpool(stderr);
		ipcdump(stderr);

		exit(0);

		}

	else if (pid==0)
		{

		sleep(5);

		sock=ipcclient(ipcaddress("localhost"),ipcport(port));
		fprintf(stderr,"%s: ipcclient(%s)=%d\n",name,port,sock);
		if (sock<0)
			perror("ipcclient"), exit(11);

		system(NETSTAT);

		fprintf(stderr,"%s: ipcsend(%d)\n",name,bytes);
		length=ipcsend(sock,buffer,bytes);
		fprintf(stderr,"%s: ipcsend(%d)=%d\n",name,bytes,length);
		if (length<=0)
			perror("ipcsend"), exit(12);

		system(NETSTAT);

		bufpool(stderr);
		kvpool(stderr);
		nmallocinfo(stderr);
		nmallocpool(stderr);
		ipcdump(stderr);

		exit(0);

		}

	else
		perror("fork"), exit(20);
	}
