/*
**	Client-Server Model, simple message exchange
**
**	Arguments:	<port number>
**
**	@(#)egstreams1.c	4.2 94/05/19 jsloan@ncar.ucar.edu
*/

#include <stdio.h>
#include "libttypes.h"
#include <sys/wait.h>
#include "ipc.h"

main(argc,argv)
int argc;
char **argv;
	{
	int pid, length;
	IPCSOCKET sock, s, r;
	char buffer[64];
	IPCADDRESS fromaddr, toaddr;
	IPCPORT fromport, toport;

	if (argc<2)
		fprintf(stderr,"usage: %s port\n",argv[0]), exit(1);

	pid=fork();
	if (pid>0)
		{
		sock=ipcserver(ipcport(argv[1]));
		fprintf(stderr,"SERVER ipcserver %d\n",sock);
		if (sock<0)
			perror("ipcserver"), exit(2);
		s=ipcaccept(sock);
		fprintf(stderr,"SERVER ipcaccept %d\n",s);
		if (s<0)
			perror("ipcaccept"), exit(3);
		ipcthisend(s,&fromaddr,&fromport);
		ipcthatend(s,&toaddr,&toport);
		fprintf(stderr,"SERVER endpoints (%#lx %d) (%#lx %d)\n",
			fromaddr,fromport,toaddr,toport);
		length=ipcvrecv(s,buffer,sizeof(buffer));
		fprintf(stderr,"SERVER ipcvrecv %d\n",length);
		if (length<=0)
			perror("ipcvrecv"), exit(4);
		fprintf(stderr,"SERVER received ``%s''\n",buffer);
		length=ipcsend(s,"SERVER",strlen("SERVER")+1);
		fprintf(stderr,"SERVER ipcsend %d\n",length);
		if (length<=0)
			perror("ipcsend"), exit(5);
		wait(NULL);
		exit(0);
		}
	else if (pid==0)
		{
		sleep(5);
		r=ipcclient(ipcaddress("localhost"),ipcport(argv[1]));
		fprintf(stderr,"CLIENT ipcclient %d\n",r);
		if (r<0)
			perror("ipcclient"), exit(11);
		length=ipcsend(r,"CLIENT",strlen("CLIENT")+1);
		fprintf(stderr,"CLIENT ipcsend %d\n",length);
		if (length<=0)
			perror("ipcsend"), exit(12);
		length=ipcvrecv(r,buffer,sizeof(buffer));
		fprintf(stderr,"CLIENT ipcvrecv %d\n",length);
		if (length<=0)
			perror("ipcvrecv"), exit(13);
		fprintf(stderr,"CLIENT received ``%s''\n",buffer);
		exit(0);
		}
	else
		perror("fork"), exit(10);
	}
