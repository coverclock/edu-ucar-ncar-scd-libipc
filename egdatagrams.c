/*
**	Peer Model, single server, multiple clients
**
**	Arguments:	<port number> <number of clients>
**
**	@(#)egdatagrams.c	4.2 94/05/19 jsloan@ncar.ucar.edu
*/

#include <stdio.h>
#include "nmalloc.h"
#include "ipc.h"

static int clients=0;

main(argc,argv)
int argc;
char **argv;
	{
	char buffer[256];
	int sock, bytes, pid, ndx;
	IPCADDRESS addr;
	IPCMBOX mbox;

	if (argc<3)
		fprintf(stderr,"usage: %s port clients\n",argv[0]), exit(1);

	clients=atoi(argv[2]);

	for (ndx=0; ndx<clients; ndx++)
		{

		pid=fork();

		if (pid==0)
			{

			sock=ipcpeer(ipcmbox("0"));
			if (sock<0)
				perror("ipcpeer"), exit(2);

			ipcthisend(sock,&addr,&mbox);
			fprintf(stderr,
				"CLIENT %d sock=%d, addr=%#lx, mbox=%d\n",
				ndx,sock,addr,mbox);

			addr=ipcaddress("localhost");
			if (addr<=0)
				perror("ipcaddress"), exit(3);

			mbox=ipcmbox(argv[1]);
			if (mbox==0)
				perror("ipcmbox"), exit(4);
			fprintf(stderr,
				"CLIENT %d toaddr=%#lx, tombox=%d\n",
				ndx,addr,mbox);

			sleep(5);

			sprintf(buffer,"CLIENT %d",ndx);

			bytes=ipcput(sock,buffer,strlen(buffer)+1,addr,mbox);
			fprintf(stderr,"CLIENT %d ipcput=%d\n",ndx,bytes);
			if (bytes<0)
				perror("ipcput"), exit(5);

			bytes=ipcget(sock,buffer,sizeof(buffer),&addr,&mbox);
			fprintf(stderr,
				"CLIENT %d fraddr=#lx frmbox=%d ipcget=%d\n",
				ndx,addr,mbox,bytes);
			if (bytes<0)
				perror("ipcput"), exit(6);

			fprintf(stderr,"CLIENT %d gets ``%s''\n",ndx,buffer);

			bufpool(stderr);
			kvpool(stderr);
			nmallocinfo(stderr);
			nmallocpool(stderr);
			ipcdump(stderr);

			exit(0);

			}
		else if (pid<0)
			perror("fork"), exit(7);
		}

	sock=ipcpeer(ipcmbox(argv[1]));
	if (sock<0)
		perror("ipcpeer"), exit(11);

	ipcthisend(sock,&addr,&mbox);
	fprintf(stderr,
		"SERVER sock=%d, addr=%#lx, mbox=%d\n",
		sock,addr,mbox);

	while (1)
		{

		bytes=ipcget(sock,buffer,sizeof(buffer),&addr,&mbox);
		fprintf(stderr,"SERVER fraddr=%#lx frmbox=%d ipcget=%d\n",
			addr,mbox,bytes);
		if (bytes<0)
			perror("ipcput"), exit(12);

		fprintf(stderr,"SERVER gets ``%s''\n",buffer);
		strcat(buffer," reply");
		bytes=ipcput(sock,buffer,strlen(buffer)+1,addr,mbox);
		fprintf(stderr,"SERVER ipcput=%d\n",bytes);
		if (bytes<0)
			perror("ipcput"), exit(13);

		}
	}
