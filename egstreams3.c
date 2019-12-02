/*
**	Client-Server Model, single server, multiple clients
**
**	Arguments:	<port number> <number of clients>
**
**	@(#)egstreams3.c	4.2 94/05/19 jsloan@ncar.ucar.edu
*/

#include <stdio.h>
#include "libttypes.h"
#include <sys/wait.h>
#include "ipc.h"

static int clients=0;
static char buffer[64];

main(argc,argv)
int argc;
char **argv;
	{
	int pid, sock, newsock, i;

	if (argc<3)
		fprintf(stderr,"usage: %s port clients\n",argv[0]), exit(1);

	clients=atoi(argv[2]);
	sock=ipcserver(ipcport(argv[1]));
	fprintf(stderr,"SERVER ipcserver=%d\n",sock);
	if (sock<0)
		perror("icpserver"), exit(2);
	for (i=0; i<clients; i++)
		{
		pid=fork();
		if (pid==0)
			client(i,argv[1]);
		else if (pid<0)
			perror("fork"), exit(3);
		}
	while (1)
		{
		fprintf(stderr,"SERVER MAIN continuing\n");
		newsock=ipcready();
		if (newsock==sock)
			{
			newsock=ipcaccept(sock);
			fprintf(stderr,"SERVER MAIN ipcaccept=%d\n",newsock);
			if (newsock<0)
				perror("ipcaccept"), exit(4);
			}
		else
			server(newsock);
		}
	}

server(sock)
int sock;
	{
	int length;

	fprintf(stderr,"SERVER %d beginning\n",sock);
	while (1)
		{
		length=ipcvrecv(sock,buffer,sizeof(buffer));
		fprintf(stderr,"SERVER %d ipcvrecv=%d\n",sock,length);
		if (length<0)
			{
			perror("ipcvrecv");
			break;
			}
		if (length==0)
			{
			fprintf(stderr,"SERVER %d disconnect\n",sock);
			break;
			}
		fprintf(stderr,"SERVER %d received ``%s''\n",sock,buffer);
		strcat(buffer," reply");
		length=ipcsend(sock,buffer,strlen(buffer)+1);
		fprintf(stderr,"SERVER %d ipcsend=%d\n",sock,length);
		if (length<0)
			{
			perror("ipcsend");
			break;
			}
		if (length==0)
			{
			fprintf(stderr,"SERVER %d disconnect\n",sock);
			break;
			}
		}
	ipcclose(sock);
	fprintf(stderr,"SERVER %d returning\n",sock);
	}

client(serial,port)
int serial;
char *port;
	{
	int sock, length;

	fprintf(stderr,"CLIENT %d starting\n",serial);
	sock=ipcclient(ipcaddress("localhost"),ipcport(port));
	fprintf(stderr,"CLIENT %d ipcclient=%d\n",serial,sock);
	if (sock<0)
		perror("ipcclient"), exit(11);
	sprintf(buffer,"CLIENT %d",serial);
	length=ipcsend(sock,buffer,strlen(buffer)+1);
	fprintf(stderr,"CLIENT %d ipcsend=%d\n",serial,length);
	if (length<0)
		perror("ipcsend"), exit(12);
	if (length==0)
		fprintf(stderr,"CLIENT %d disconnect\n",serial), exit(13);
	length=ipcvrecv(sock,buffer,sizeof(buffer));
	fprintf(stderr,"CLIENT %d ipcvrecv=%d\n",serial,length);
	if (length<0)
		perror("ipcsend"), exit(14);
	if (length==0)
		fprintf(stderr,"CLIENT %d disconnect\n",serial), exit (15);
	fprintf(stderr,"CLIENT %d received ``%s''\n",serial,buffer);
	ipcclose(sock);
	fprintf(stderr,"CLIENT %d exiting\n",serial);

	bufpool(stderr);
	kvpool(stderr);
	nmallocinfo(stderr);
	nmallocpool(stderr);
	ipcdump(stderr);

	exit(0);
	}
