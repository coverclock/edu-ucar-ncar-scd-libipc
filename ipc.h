/*
**	I N T E R P R O C E S S   C O M M U N I C A T I O N
**
**	Copyright 1990, 1991, 1992, 1993, 1994, 1995 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	@(#)ipc.h	4.11 96/06/11 jsloan@ncar.ucar.edu
**
*/

#ifndef H_IPCCOMM
#define H_IPCCOMM

#include <stdio.h>
#include "libtbase.h"
#include "libtparam.h"
#include "ipcproto.h"

typedef libt_unsigned32 IPCADDRESS;
typedef libt_unsigned16 IPCPORT;
typedef libt_unsigned16 IPCMBOX;
typedef int IPCSOCKET;

#define IPCHOSTNAMELEN	(MAXHOSTNAMELEN+1)
#define IPCADDRESSLEN	(32)

extern IPCADDRESS ipcaddress IPCPROTO((char *host));

extern IPCADDRESS ipcaddresses IPCPROTO((char *host, int ndx));

extern int ipclocal IPCPROTO((IPCADDRESS unknown));

extern char *ipchost IPCPROTO((char name[IPCHOSTNAMELEN]));

extern IPCPORT ipcport IPCPROTO((char *service));

extern IPCMBOX ipcmbox IPCPROTO((char *service));

extern IPCSOCKET ipcserver IPCPROTO((IPCPORT port));

extern IPCSOCKET ipcoserver IPCPROTO((IPCPORT port, int shift, int sndbuf, int rcvbuf, int delay));

extern IPCSOCKET ipcaccept IPCPROTO((IPCSOCKET sock));

extern IPCSOCKET ipcclient IPCPROTO((IPCADDRESS address, IPCPORT port));

extern IPCSOCKET ipcoclient IPCPROTO((IPCADDRESS address, IPCPORT port, int shift, int sndbuf, int rcvbuf, int delay));

extern IPCSOCKET ipcpeer IPCPROTO((IPCMBOX mbox));

extern int ipcsend IPCPROTO((IPCSOCKET sock, char *buf, int len));

extern int ipcvsend IPCPROTO((IPCSOCKET sock, char *buf, int len));

extern int ipcrecv IPCPROTO((IPCSOCKET sock, char *buf, int len));

extern int ipctrecv IPCPROTO((IPCSOCKET sock, char *buf, int len, int seconds));

extern int ipcvrecv IPCPROTO((IPCSOCKET sock, char *buf, int len));

extern int ipcget IPCPROTO((IPCSOCKET sock, char *buf, int len, IPCADDRESS *addrp, IPCMBOX *mboxp));

extern int ipcput IPCPROTO((IPCSOCKET sock, char *buf, int len, IPCADDRESS addr, IPCMBOX mbox));

extern int ipcclose IPCPROTO((IPCSOCKET sock));

extern int ipcdaemon IPCPROTO((void));

extern IPCSOCKET ipcready IPCPROTO((void));

extern IPCSOCKET ipcpoll IPCPROTO((void));

extern IPCSOCKET ipctimed IPCPROTO((int seconds));

extern IPCSOCKET ipcpair IPCPROTO((IPCSOCKET *sockp));

extern void ipcregister IPCPROTO((IPCSOCKET sock));

extern void ipcunregister IPCPROTO((IPCSOCKET sock));

extern void ipcderegister IPCPROTO((void));

extern int ipcthisend IPCPROTO((IPCSOCKET sock, IPCADDRESS *addrp, IPCPORT *portp));

extern int ipcthatend IPCPROTO((IPCSOCKET sock, IPCADDRESS *addrp, IPCPORT *portp));

extern char *ipcformat IPCPROTO((IPCADDRESS addr, IPCPORT port, char *buffer));

extern int ipcnonblocking IPCPROTO((IPCSOCKET sock));

extern int ipcblocking IPCPROTO((IPCSOCKET sock));

extern int ipcsiginc IPCPROTO((int sig, int *ctr));

extern int ipcsigdec IPCPROTO((int sig, int *ctr));

extern void ipcdump IPCPROTO((FILE *fp));

extern int ipcparanoid IPCPROTO((IPCSOCKET sock));

extern int ipcdebug IPCPROTO((IPCSOCKET sock));

extern int ipcnodebug IPCPROTO((IPCSOCKET sock));

extern int ipclinger IPCPROTO((IPCSOCKET sock));

extern int ipcnolinger IPCPROTO((IPCSOCKET sock));

extern int ipcproduct IPCPROTO((int mbs, int ms));

extern int ipcoptimal IPCPROTO((int product, int *bufsizp));

extern IPCSOCKET locserver IPCPROTO((char *path));

extern IPCSOCKET locclient IPCPROTO((char *path));

extern IPCSOCKET locaccept IPCPROTO((IPCSOCKET sock));

extern int ipcoptions IPCPROTO((IPCSOCKET sock, int *vector, int *vsize));

#endif /* !H_IPCCOMM */
