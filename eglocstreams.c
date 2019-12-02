/*
**	Client-Server Model, single server, multiple clients
**
**	Arguments:	<path name> <number of clients>
**
**	@(#)eglocstreams.c	1.1 94/09/30 jsloan@ncar.ucar.edu
*/

#include <stdio.h>
#include "libttypes.h"
#include <sys/wait.h>
#include "ipc.h"

static char *path;
static int clients=0;
static char buffer[64];

main(argc,argv)
int argc;
char **argv;
	{
	int pid, sock, newsock, i;

	if (argc<3)
		fprintf(stderr,"usage: %s path clients\n",argv[0]), exit(1);

	clients=atoi(argv[2]);
	path=argv[1];
	sock=locserver(path);
	fprintf(stderr,"SERVER locserver(%s)=%d\n",path,sock);
	if (sock<0)
		perror("locserver"), exit(2);
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
			newsock=locaccept(sock);
			fprintf(stderr,"SERVER MAIN locaccept=%d\n",newsock);
			if (newsock<0)
				perror("locaccept"), exit(4);
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

client(serial,path)
int serial;
char *path;
	{
	int sock, length;

	fprintf(stderr,"CLIENT %d starting\n",serial);
	sock=locclient(path);
	fprintf(stderr,"CLIENT %d ipcclient(%s)=%d\n",serial,path,sock);
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
