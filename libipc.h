/*
**	L I B I P C
**
**	Copyright 1990, 1991, 1992, 1994, 1995 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	@(#)libipc.h	4.13 95/12/24 jsloan@ncar.ucar.edu
**
**	For internal use only.
**	Not recommended for inclusion by applications which use LIBIPC.
*/

#ifndef H_LIBIPC
#define H_LIBIPC

#ifndef PROTOTYPE_LIBTOOLS
#define PROTOTYPE_LIBTOOLS
#endif /* PROTOTYPE_LIBTOOLS */
#ifndef PROTOTYPE_LIBIPC
#define PROTOTYPE_LIBIPC
#endif /* PROTOTYPE_LIBIPC */

#include "serialize.h"
#include <stdio.h>
#include <fcntl.h>
#include <sys/time.h>
#include "libttypes.h"
#include <sys/socket.h>
#include <sys/file.h>
#include <sys/un.h>
#include "libtin.h"
#include <netdb.h>
#include <arpa/inet.h>
#ifndef UNICOS_LIBIPC
#include <sys/ipc.h>
#include <sys/msg.h>
#endif /* !UNICOS_LIBIPC */
#include <sys/param.h>
#include <signal.h>
#include "errors.h"
#include "ipc.h"

#define IPCLOCALHOST	"localhost"

/*
** N.B. For Linux 1.2.13
*/
#ifndef SOMAXCONN
#define SOMAXCONN	(5)
#endif /* SOMAXCONN */

/*
** N.B. This option has never been tested in production.
*/
#ifdef SENDRECV_LIBIPC

#define IPCSEND(s,b,n)	send((s),(b),(n),0)
#define IPCRECV(s,b,n)	recv((s),(b),(n),0)

#else /* SENDRECV_LIBIPC */

#define IPCSEND(s,b,n)	write((s),(b),(n))
#define IPCRECV(s,b,n)	read((s),(b),(n))

#endif /* SENDRECV_LIBIPC */

#ifdef PTHREADS_LIBTOOLS

extern const SEMAPHORE *sem_ipcready_libi;
extern const SEMAPHORE *sem_ipcaddress_libi;
extern const SEMAPHORE *sem_ipcport_libi;

extern void libipc(void);
extern void libipcdump(FILE *fp);

#define P(sem)          (libipc(),semP((SEMAPHORE *)sem))
#define V(sem)          (libipc(),semV((SEMAPHORE *)sem))
#define SW()		(semSw())

#else /* PTHREADS_LIBTOOLS */

#define P(sem)          (0)
#define V(sem)          (0)
#define SW()		(0)

#endif /* PTHREADS_LIBTOOLS */

#endif /* !H_LIBIPC */
