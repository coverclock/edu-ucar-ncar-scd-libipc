/*
**	E G I P C O C S
**
**	Copyright 1995-1996 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	Title		Example IPC Optimized Client Server
**	Program		egipcocs
**	Project		libipc
**	Author		John Sloan
**	Email		jsloan@ncar.ucar.edu
**	Date		Mon Jul 31 13:00:41 MDT 1995
**	Organization	NCAR/SCD, P.O. Box 3000, Boulder CO 80307
**
**	Abstract
**
**
*/

static char copyright[]="Copyright 1995-1996 University Corporation for Atmospheric Research - All Rights Reserved";
static char sccsid[]="@(#)egipcocs.c	1.9 96/06/13 jsloan@ncar.ucar.edu";

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include "errors.h"
#include "ipc.h"
#include "nmalloc.h"
#include "timeclock.h"

#define USAGE	"[ -b mb/s ] [ -c ] [ -d ] [ -h host ] [ -k kb ] [ -l ms ] [ -n ] [ -p ] [ -s service ] [ -v ] [ -w mb ]"

#define ONEKB		(1024)
#define ONEMB		(1024*1024)

static char *argvzero="unknown";
static int debug=0;
static int verify=0;
static int print=0;

/*
**	report: report on how a socket is currently configured.
*/
static void
report(IPCSOCKET sock)
	{
	int vector[11], vsize;

	vsize=sizeof(vector);
	(void)ipcoptions(sock,vector,&vsize);

	if (debug)
		{
		fprintf(stderr,
			"%s: TCP_MAXSEG=%d TCP_WINSHIFT=(0x%x,%d,%d)\n",
			argvzero,
			vector[0],
			vector[8],vector[9],vector[10]
			);
		fprintf(stderr,
			"%s: SO_SNDBUF=%u(0x%x) SO_RCVBUF=%u(0x%x)\n",
			argvzero,
			vector[1],vector[1],vector[2],vector[2]
			);
		fprintf(stderr,
		"%s: TCP_NODELAY=0x%x TCP_DEBUG=0x%x TCP_KEEPALIVE=0x%x\n",
			argvzero,
			vector[6],vector[3],vector[4]
			);
		fprintf(stderr,
		"%s: TCP_LINGER=0x%x TCP_RFC1323=0x%x\n",
			argvzero,
			vector[5],vector[7]
			);
		}
	}

/*
**	allocate: get a buffer.
*/
static void *
allocate(int kb)
	{
	void *buffer;

	if ((buffer=namalloc(kb*ONEKB))==NULL)
		{
		perror("namalloc");
		exit(4);
		}

	return(buffer);
	}

/*
**	reader: read data from a socket.
*/
static int
reader(IPCSOCKET sock, int kb)
	{
	void *buffer;
	int rc, blocks, bytes;
	TIMESTAMP before, after;

	buffer=allocate(kb);
	blocks=0;
	bytes=0;
	before=timeclock();
	for (;;)
		{
		if ((rc=ipcrecv(sock,buffer,kb*ONEKB))<0)
			{
			perror("ipcsend");
			return(-1);
			}
		if (rc==0)
			break;
		bytes+=rc;
		while (bytes>=(kb*ONEKB))
			{
			blocks++;
			bytes-=(kb*ONEKB);
			}
		}
	after=timeclock();
	if (debug||print)
		{
		long secs;
		double bw, mbytes;

		mbytes=(((double)kb*(double)ONEKB*(double)blocks)+(double)bytes)
			/(double)ONEMB;
		secs=after-before;
		bw=(mbytes*8.0)/(double)secs;

		if (debug)
			fprintf(stderr,"%s reader: %fMB/%dsecs=%fMb/s\n",
				argvzero,mbytes,secs,bw);
		if (print)
			printf(" r %f %d %f",mbytes,secs,bw);
		}

	return(0);
	}

/*
**	writer: write data to a socket.
*/
static int
writer(IPCSOCKET sock, int kb, int mb)
	{
	void *buffer;
	int rc, blocks, bytes, k, mbytes;
	TIMESTAMP before, after;

	buffer=allocate(kb);
	blocks=0;
	bytes=0;
	mbytes=0;
	before=timeclock();
	while (mb>0)
		{
		if ((rc=ipcsend(sock,buffer,kb*ONEKB))<0)
			{
			perror("ipcsend");
			return(-1);
			}
		bytes+=rc;
		while (bytes>=(kb*ONEKB))
			{
			blocks++;
			bytes-=(kb*ONEKB);
			}
		mbytes+=rc;
		if (mbytes>=ONEMB)
			{
			mbytes-=ONEMB;
			mb--;
			}
		}
	after=timeclock();
	if (debug||print)
		{
		long secs;
		double bw, mbytes;

		mbytes=(((double)kb*(double)ONEKB*(double)blocks)+(double)bytes)
			/(double)ONEMB;
		secs=after-before;
		bw=(mbytes*8.0)/(double)secs;

		if (debug)
			fprintf(stderr,"%s writer: %fMB/%dsecs=%fMb/s\n",
				argvzero,mbytes,secs,bw);

		if (print)
			printf(" w %f %d %f",mbytes,secs,bw);
		}

	return(0);
	}

/*
**	client: set up a client socket and connect to a server.
*/
static int
client(int kb, int mb, char *host, char *service, int shift, int sndbuf, int rcvbuf, int nodelay)
	{
	IPCADDRESS addr;
	IPCSOCKET socket;
	IPCPORT port;
	char name[64];
	int rc;

	if (debug)
		fprintf(stderr,"%s: client(%d,%d,``%s'',``%s'',%d,%d,%d,%d)\n",
			argvzero,kb,mb,host,service,shift,sndbuf,
			rcvbuf,nodelay);

	addr=ipcaddress(host);
	port=ipcport(service);
	(void)ipcformat(addr,port,name);
	if (debug)
		fprintf(stderr,"%s client: address=%s\n",
			argvzero,name);
	if (addr==0L)
		{
		perror("client ipcaddress");
		return(-1);
		}
	if (port==0)
		{
		perror("client ipcport");
		return(-2);
		}

	if ((sndbuf==0)&&(rcvbuf==0))
		socket=ipcclient(addr,port);
	else
		socket=ipcoclient(addr,port,shift,sndbuf,rcvbuf,nodelay);
	if (debug)
		fprintf(stderr,"%s client: socket=%d\n",argvzero,socket);
	if (socket<0)
		{
		perror("client ipcclient");
		return(-3);
		}
	report(socket);

	if (print)
		printf(" c 0x%x %d %s %d %d",addr,port,name,socket,socket);

	if (mb>0)
		rc=writer(socket,kb,mb);
	else
		rc=reader(socket,kb);

	return(rc);
	}

/*
**	server: set up a server socket and wait for a client to connect.
*/
static int
server(int kb, int mb, char *service, int shift, int sndbuf, int rcvbuf, int nodelay)
	{
	IPCSOCKET socket, socket2;
	IPCADDRESS addr;
	IPCPORT port;
	char name[64], hostname[64];
	int rc;

	if (debug)
		fprintf(stderr,"%s: server(%d,%d,``%s'',%d,%d,%d,%d)\n",
			argvzero,kb,mb,service,shift,sndbuf,
			rcvbuf,nodelay);

	addr=ipcaddress(ipchost(hostname));
	port=ipcport(service);
	(void)ipcformat(addr,port,name);
	if (debug)
		fprintf(stderr,"%s server: address=%s\n",
			argvzero,name);
	if (port==0)
		{
		perror("server ipcport");
		return(-1);
		}
	
	if ((sndbuf==0)&&(rcvbuf==0))
		socket=ipcserver(port);
	else
		socket=ipcoserver(port,shift,sndbuf,rcvbuf,nodelay);
	if (debug)
		fprintf(stderr,"%s server: socket=%d\n",argvzero,socket);
	if (socket<0)
		{
		perror("server ipcserver");
		return(-2);
		}
	report(socket);

	socket2=ipcaccept(socket);
	if (debug)
		fprintf(stderr,"%s server: socket2=%d\n",argvzero,socket2);
	if (socket2<0)
		{
		perror("server ipcaccept");
		return(-3);
		}
	report(socket);

	if (print)
		printf(" s 0x%x %d %s %d %d",addr,port,name,socket,socket2);

	if (mb>0)
		rc=writer(socket2,kb,mb);
	else
		rc=reader(socket2,kb);

	return(rc);
	}

/*
**	M A I N   P R O G R A M
*/
main(int argc, char **argv)
	{
	int opt;
	extern char *optarg;
	extern int optind;
	int inerror, usage;
	FILE *fp;

	char *host, *service, hostname[IPCHOSTNAMELEN];
	int doclient, bandwidth, latency, mb;
	int nodelay, shift, sndbuf, rcvbuf;
	int rc, product, bufsiz, kb;
	IPCSOCKET sock;

	/*
	** Crack the command line and pick out the options.
	*/

	argvzero=((argvzero=strrchr(argv[0],'/'))==NULL)?argv[0]:argvzero+1;
	usage=0;
	debug=0;
	verify=0;
	doclient=0;
	bandwidth=0;
	latency=0;
	host="localhost";
	service="0";
	mb=0;
	kb=1;
	nodelay=0;
	print=0;
	while ((opt=getopt(argc,argv,"b:cdh:k:l:nps:vw:"))>=0)
		{
		inerror=0;
		switch (opt)
			{
		case 'b':
			bandwidth=atoi(optarg);
			break;
		case 'c':
			doclient=1;
			break;
		case 'd':
			debug=1;
			break;
		case 'h':
			host=optarg;
			break;
		case 'k':
			kb=atoi(optarg);
			break;
		case 'l':
			latency=atoi(optarg);
			break;
		case 'n':
			nodelay=1;
			break;
		case 'p':
			print=1;
			break;
		case 's':
			service=optarg;
			break;
		case 'v':
			verify=1;
			break;
		case 'w':
			mb=atoi(optarg);
			break;
		default:
			usage++;
			}
		if (inerror>0)
			{
			fprintf(stderr,"%s: bad value -- -%c %s\n",
				argvzero,opt,optarg);
			usage++;
			}
		}
	if (usage)
		{
		fprintf(stderr,"usage: %s %s\n",argvzero,USAGE);
		exit(1);
		}

	if (debug)
		fprintf(stderr,"%s: %s%s%s%s%s%s:%s %dKB %dMB %dMb/s %dms\n",
			argvzero,debug?"debug ":"",print?"print ":"",
			verify?"verify ":"",doclient?"client ":"server ",
			nodelay?"nodelay ":"",host,service,kb,mb,
			bandwidth,latency);

	if (print)
		printf("%s %s %d %s %s %s %d %d %d %d",
			argvzero,ipchost(hostname),getpid(),nodelay?"n":"d",
			host,service,kb,mb,bandwidth,latency);

	product=ipcproduct(bandwidth,latency);
	shift=ipcoptimal(product,&bufsiz);
	sndbuf=bufsiz;
	rcvbuf=bufsiz;

	if (debug)
		fprintf(stderr,
	"%s: product=%d shift=%d sndbuf=%u(0x%x) rcvbuf=%u(0x%x)\n",
			argvzero,product,shift,sndbuf,sndbuf,rcvbuf,rcvbuf);

	if (print)
		printf(" %d %d %d 0x%x %d 0x%x",
			product,shift,sndbuf,sndbuf,rcvbuf,rcvbuf);

	rc=doclient?
		client(kb,mb,host,service,shift,sndbuf,rcvbuf,nodelay)
		:
		server(kb,mb,service,shift,sndbuf,rcvbuf,nodelay)
		;

	if (debug)
		fprintf(stderr,"%s: %s=%d\n",
			argvzero,doclient?"client":"server",rc);

	if (print)
		putchar('\n');

	exit(rc<0?0:2);
	}
