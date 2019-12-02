########################################################################
#
#			I P C   L I B R A R Y
#
#				4 . 3
#
#			jsloan@ncar.ucar.edu
#
#	Copyright 1990-1996 University Corporation for Atmospheric Research
#			 All Rights Reserved
#
#	@(#)Makefile	4.34 96/06/20 jsloan@ncar.ucar.edu
#
#	Releases of this package have been tested on the following
#	platforms. However, because many of these machines were loaners
#	and therefore only available for a limited time, the portability
#	of all releases has not been established.
#
#		SMI Sun-4	SunOS 4.1
#		SMI Sun-3	SunOS 4.0
#		SMI Sun-3	SunOS 4.0 GCC
#		DEC VAX 8550	Ultrix-32 V3.0
#		IBM 370/4381	AIX/370 1.2
#		IBM RS6000/550	AIX 3.1, 3.2
#		H-P HP9000/720	HP-UX A.B8.05
#		SGI IP19	IRIX 5.3
#		CRI CRAY YMP8	UNICOS 8.0.3
#		Dell P133c XPS	Linux 2.1.13
#		SMI Ultra-2	Solaris 5.5.1
#		NEC SX-4	Super-UX 6.1
#
#	The modules ipcoserver and ipcoclient contain options to
#	optimize socket connections between client and servers by
#	using large socket buffers and RFC1323 TCP window scaling
#	if available. For more information on RFC1323 and related
#	issues, see the text files irwin.txt, mahdavi.txt, and
#	welch.txt.
#
########################################################################

ROOT=		/crestone/home/jsloan
NAME=		libipc

########################################################################
#	Symbolic Definitions 
########################################################################

INCLUDE=	$(ROOT)/include
LIB=		$(ROOT)/lib
PRIVATE=	$(NAME).a
PUBLIC=		$(LIB)/$(PRIVATE)
LIBS=		$(PUBLIC) $(LIB)/libtools.a -lm -lcurses -ltermcap
BIN=		$(ROOT)/bin
MAN=		$(ROOT)/man
TMPDIR=		.

DCENAME=	libipcdce
DCEPRIVATE=	$(DCENAME).a
DCEPUBLIC=	$(LIB)/$(DCEPRIVATE)
DCELIBS=	$(DCEPUBLIC) $(LIB)/libtoolsdce.a -lm -lcurses -ltermcap -lpthreads

########################################################################
#	File Definition
########################################################################

HEADERS=	ipc.h ipcproto.h libipc.h

SOURCES=	ipcaccept.c ipcaddress.c ipcclient.c \
		ipcclose.c ipcdaemon.c ipcformat.c ipcget.c \
		ipchost.c ipclocal.c ipcmustswap.c ipcoptions.c \
		ipcoclient.c ipcoserver.c ipcoptimal.c \
		ipcpair.c ipcpeer.c ipcport.c ipcput.c ipcready.c \
		ipcrecv.c ipcsend.c ipcserver.c ipcsigdec.c ipcsiginc.c \
		ipcthatend.c ipcthisend.c ipctimeout.c ipctrecv.c \
		ipcvrecv.c ipcvsend.c \
		libipc.c locaccept.c locclient.c locserver.c

EGSOURCES=	egdatagrams.c egdgmclient.c egdgmserver.c \
		egipcclient.c egipcoptions.c egipcserver.c \
		egstreams1.c egstreams2.c egstreams3.c egstreams4.c \
		eglocstreams.c egipcocs.c egipcaddresses.c \
		egipcoptimal.c

EGSCRIPTS=	egstreams4.sh egipcocs.sh

MANPAGES=	ipc.3l

MISC=		ABSTRACT README LICENSE Makefile \
		sccsunbundle cpdate \
		irwin.txt mahdavi.txt rfc1323.txt tn377ia.me welch.txt

DISTRIBUTION=	$(MISC) $(HEADERS) $(SOURCES) $(EGSOURCES) $(EGSCRIPTS) \
		$(MANPAGES)

OBJECTS=	ipcaccept.o ipcaddress.o \
		ipcclient.o ipcclose.o \
		ipcdaemon.o \
		ipcformat.o \
		ipcget.o \
		ipchost.o \
		ipclocal.o \
		ipcmustswap.o \
		ipcoclient.o ipcoserver.o ipcoptimal.o \
		ipcpair.o ipcoptions.o ipcpeer.o ipcport.o ipcput.o \
		ipcready.o ipcrecv.o \
		ipcsend.o ipcserver.o ipcsigdec.o ipcsiginc.o \
		ipcthatend.o ipcthisend.o ipctimeout.o ipctrecv.o \
		ipcvrecv.o ipcvsend.o \
		libipc.o locaccept.o locclient.o locserver.o

EXAMPLES=	egdatagrams egdgmclient egdgmserver \
		egipcclient egipcoptions egipcserver \
		egstreams1 egstreams2 egstreams3 egstreams4 \
		eglocstreams egipcocs egipcaddresses \
		egipcoptimal

EXAMPLE=	egdummy

TARGET=		install

########################################################################
#	Option Definitions
########################################################################
#
#	It is the stuff in this section that you may want to peruse
#	and modify in order to tailor libipc for your particular
#	configuration. Some combinations of options that have worked
#	on various platforms in the past are commented out. These
#	may offer some useful guidance.
#
#	The following compiler symbols may be defined while building
#	libipc to alter the resulting product. (For the most part,
#	the prefix of each symbol indicates which module it affects.)
#	If the symbol is in the form <MODULE>_<OPTION> then it affects
#	a single module and is probably used only in the .c file. If the
#	symbol is of the form <OPTION>_LIBIPC or <OPTION>_LIBTOOLS then it
#	affects many modules, may be used in .h files, and can be used when
#	compiling applications which include the .h files.
#
#	ERRNO_LIBTOOLS		Use errno functions instead of errno variable
#	NMALLOC_LIBTOOLS	Use libtools "new" malloc functions
#	PROTOTYPE_LIBIPC	C compiler groks ANSI function prototypes
#	PTHREADS_LIBTOOLS	Applications will use OSF DCE Threads
#	SELECT_LIBIPC		Need to include sys/select.h
#	SENDRECV_LIBIPC		Use send/receive vs. read/write (untested)
#	UNICOS_LIBIPC		Use UNICOS include files
#
#	For the record, I normally try to write everything in ANSI C
#	with function prototypes, and stick to POSIX compliant stuff.
#	Libtools _must_ be installed using an ANSI C compiler that
#	supports function prototypes. However, applications using
#	libipc may or may not use function prototypes, depending on
#	the definition of PROTOTYPE_LIBIPC.
#
#	Below, CONDITIONALS are applied to applications built using
#	libipc, and CUSTOMIZATIONS are applied to the installation
#	of libipc itself.
#
########################################################################

CUSTOMIZATIONS=
#CUSTOMIZATIONS=-DSELECT_LIBIPC
#CUSTOMIZATIONS=-DSELECT_LIBIPC -DPTHREADS_LIBTOOLS

CONDITIONALS=-DPROTOTYPE_LIBTOOLS -DPROTOTYPE_LIBIPC

DEBUG=-g
#DEBUG=-O
CC=gcc ### Gnu CC
CSYSTEM=
CFLAGS=$(DEBUG) $(CSYSTEM) $(CUSTOMIZATIONS)
CINCS=-I$(INCLUDE)

AR=ar
ARFLAGS=rv
RANLIB=ranlib
#RANLIB=ar tvs # SGI

LDSYSTEM=
LDFLAGS=$(DEBUG) $(LDSYSTEM)
LDLIBS=$(LIBS)

COMPRESS=gzip -fv

########################################################################
#	default target
########################################################################

all:	$(PRIVATE)

########################################################################
#	Predefined Architecture Specific Targets
#
#	N.B.	Some of these configurations were added to this
#		Makefile after the ports were complete and the
#		platforms were no longer available to be tested
#		upon. Hence, there may be some syntax errors.
#		Also, later releases of the various operating
#		systems may require modifications.
########################################################################

STAT_DEFS=	

##
##	SMI/SPARC/SunOS w/o DCE
##

SUNOS_DEFS=	

sunos:
	make $(TARGET) \
		ROOT=$(ROOT) \
		CUSTOMIZATIONS="$(STAT_DEFS) $(SUNOS_DEFS)" \
		EXAMPLE=$(EXAMPLE) \
		CC=gcc \
		DEBUG="$(DEBUG)"

##
##	SMI/SPARC/SOLARIS w/o DCE
##

SOLARIS_DEFS=

SOLARIS_LIBS=	-lsocket -lnsl

solaris:
	make $(TARGET) \
		ROOT=$(ROOT) \
		CUSTOMIZATIONS="$(STAT_DEFS) $(SOLARIS_DEFS)" \
		EXAMPLE=$(EXAMPLE) \
		LIBS="$(LIBS) $(SOLARIS_LIBS)" \
		CC="/opt/SUNWspro/bin/cc" \
		DEBUG="$(DEBUG)"

##
##	IBM/RS6K/AIX w/o DCE
##

AIX_DEFS=	-DSELECT_LIBIPC

aix:
	make $(TARGET) \
		ROOT=$(ROOT) \
		CUSTOMIZATIONS="$(AIX_DEFS)" \
		EXAMPLE=$(EXAMPLE) \
		DEBUG="$(DEBUG)" \
		CC="cc -qlanglvl=ansi -qro -qroconst"

##
##	IBM/RS6K/AIX w. DCE
##

AIXDCE_DEFS=	$(AIX_DEFS) -DPTHREADS_LIBTOOLS

AIXDCE_LIBS=	

aixdce:
	make $(TARGET) \
		ROOT=$(ROOT) \
		CUSTOMIZATIONS="$(STAT_DEFS) $(AIX_DEFS)" \
		NAME="$(DCENAME)" \
		LIBS="$(DCELIBS)" \
		EXAMPLE=$(EXAMPLE) \
		CC=cc \
		DEBUG="$(DEBUG)"

##
##	DEC/ALPHA/OSF1 w/o DCE
##

OSF1_DEFS=	-DSELECT_LIBIPC

osf1:
	make $(TARGET) \
		ROOT=$(ROOT) \
		CUSTOMIZATIONS="($STAT_DEFS) $(OSF1_DEFS)" \
		EXAMPLE=$(EXAMPLE) \
		CC=cc \
		DEBUG="$(DEBUG)"

##
##	DEC/ALPHA/OSF1 w. DCE
##

OSF1DCE_DEFS=	$(OSF1_DEFS) -DPTHREADS_LIBTOOLS

OSF1DCE_LIBS=	-lpthreads -lcmalib -lmach

osf1dce:
	make $(TARGET) \
		ROOT=$(ROOT) \
		CUSTOMIZATIONS="($STAT_DEFS) $(OSF1DCE_DEFS)" \
		NAME="$(DCENAME)" \
		LIBS="$(DCELIBS) $(OSF1DCE_LIBS)" \
		EXAMPLE=$(EXAMPLE) \
		CC=cc \
		DEBUG="$(DEBUG)"

##
##	CRI/YMP/UNICOS w/o DCE
##
##	N.B.	Ignore warnings from the UNICOS C compiler of the form
##
##		cc-172 cc: WARNING File = <somefile>.c, Line = <someline>
##		No code is generated for a superfluous expression.
##
##	This is the result of some code that is no-op'ed out if
##	DCE is not enabled; the P and V semaphore operations
##	are conditionally compiled as "{}" to preserve correct
##	syntax. The compiler is doing exactly what is intended.
##

UNICOS_DEFS=	-DSELECT_LIBIPC -DUNICOS_LIBIPC

unicos:
	make $(TARGET) \
		ROOT=$(ROOT) \
		CUSTOMIZATIONS="$(STAT_DEFS) $(UNICOS_DEFS)" \
		EXAMPLE=$(EXAMPLE) \
		CC="cc -T," \
		RANLIB="ls -l" \
		DEBUG="$(DEBUG)"

##
##	SGI/IRIX w/o DCE
##

IRIX_DEFS=	-DSELECT_LIBIPC

irix:
	make $(TARGET) \
		ROOT=$(ROOT) \
		CUSTOMIZATIONS="$(IRIX_DEFS)" \
		EXAMPLE=$(EXAMPLE) \
		DEBUG="$(DEBUG)" \
		CC="cc" \
		RANLIB="ar tvs"

##
##	NEC/SX-4/SUPER-UX w/o DCE
##

SUX_DEFS=	

sux:
	make $(TARGET) \
		ROOT=$(ROOT) \
		CUSTOMIZATIONS="$(STAT_DEFS) $(SUX_DEFS)" \
		EXAMPLE=$(EXAMPLE) \
		DEBUG="$(DEBUG)" \
		CC="cc" \
		RANLIB="ar tvs"

##
##	P5/Linux w/o DCE
##

LINUX_DEFS=	

linux:
	make $(TARGET) \
		ROOT=$(ROOT) \
		CUSTOMIZATIONS="$(STAT_DEFS) $(LINUX_DEFS)" \
		EXAMPLE=$(EXAMPLE) \
		DEBUG="$(DEBUG)" \
		CC="cc" \
		RANLIB="ar tvs"

########################################################################
#	application targets
########################################################################

install:	all include $(PUBLIC)

examples:
	for F in $(EXAMPLES); do make example CUSTOMIZATIONS="$(CUSTOMIZATIONS) $(CONDITIONALS)" LIBS="$(LIBS)" EXAMPLE=$$F ROOT=$(ROOT) DEBUG="$(DEBUG)" CC="$(CC)"; done

example:
	make $(EXAMPLE) CUSTOMIZATIONS="$(CUSTOMIZATIONS) $(CONDITIONALS)" LIBS="$(LIBS)" EXAMPLE=$(EXAMPLE) ROOT=$(ROOT) DEBUG="$(DEBUG)" CC="$(CC)"

manpages:	unbundle
	cp *.3l $(MAN)/man3

distribution:	unbundle $(DISTRIBUTION)
	tar cvf - $(DISTRIBUTION) > $(TMPDIR)/$(NAME).tar
	$(COMPRESS) $(TMPDIR)/$(NAME).tar

technote.out:	tn377ia.me
	soelim tn377ia.me | eqn | expand | tbl | ditroff -me > technote.out

technote:	technote.out
	dimp -t technote.out | ipr -Pscdimagen -Limpress

## If your target system supports Sun RPC, you can try
## building the example DCE RPC applications which use parcels
## with remote procedure calls.
egrpc:	egrpcserver egrpcclient

egrpcserver:	egrpcserver.c egrpcserver.h egrpcserver_svc.c
	$(CC) $(CFLAGS) $(CINCS) -o egrpcserver egrpcserver.c egrpcserver_svc.c $(LDLIBS)

egrpcclient:	egrpcclient.c egrpcserver.h egrpcserver_clnt.c
	$(CC) $(CFLAGS) $(CINCS) -o egrpcclient egrpcclient.c egrpcserver_clnt.c $(LDLIBS)

egrpcserver.h egrpcserver_svc.c egrpcserver_clnt.c:	egrpcserver.x
	rpcgen egrpcserver.x

## If your target system supports DCE RPC, you can try
## building the example DCE RPC applications which use parcels
## with remote procedure calls.
egdce:	egdceserver egdceclient

egdceserver:	egdceserver.c egdceserver.h egdceserver_sstub.o
	$(CC) -DIDL_PROTOTYPES $(CFLAGS) $(CINCS) -o egdceserver egdceserver.c egdceserver_sstub.o $(LDLIBS) -ldce -lpthreads

egdceclient:	egdceclient.c egdceserver.h egdceserver_cstub.o 
	$(CC) -DIDL_PROTOTYPES $(CFLAGS) $(CINCS) -o egdceclient egdceclient.c egdceserver_cstub.o $(LDLIBS) -ldce -lpthreads

egdceserver.h egdceserver_sstub.o egdceserver_cstub.o:	egdceserver.idl
	idl $(CINCS) -cc_opt "$(CFLAGS)" -keep all -v egdceserver.idl

########################################################################
#	utility targets
########################################################################

clean:
	rm -f $(PRIVATE) *.o a.out core makefile technote.out
	-sccs clean

clean-examples:
	rm -f $(EXAMPLES)

depend:	makefile

makefile:	unbundle Makefile
	cat Makefile > makefile
	$(CC) $(CINCS) -M *.c >> makefile

unbundle:	sccsunbundle
	-sh sccsunbundle

lint:	unbundle
	lint -abchu $(CINCS) *.c

include:
	-mkdir $(INCLUDE)
	for h in $(HEADERS);do make $(INCLUDE)/$$h HEADER=$$h ROOT=$(ROOT);done

lib:	$(PRIVATE)

########################################################################
#	maintenance targets
########################################################################

$(DISTRIBUTION):
	-test -f $@ || sccs get $@

$(INCLUDE)/$(HEADER):	cpdate $(HEADER)
	-mkdir $(INCLUDE)
	-chmod u+w $(INCLUDE)/$(HEADER)
	-sh cpdate $(INCLUDE)/$(HEADER)
	cp $(HEADER) $(INCLUDE)/$(HEADER)
	chmod u-w $(INCLUDE)/$(HEADER)

install-object: $(OBJECT)
	$(AR) $(ARFLAGS) $(PUBLIC) $(OBJECT)
	$(RANLIB) $(PUBLIC)

$(PRIVATE):	$(OBJECTS)
	$(AR) $(ARFLAGS) $(PRIVATE) $(OBJECTS)
	$(RANLIB) $(PRIVATE)

$(PUBLIC):	cpdate $(PRIVATE)
	-mkdir $(LIB)
	-sh cpdate $(PUBLIC)
	cp $(PRIVATE) $(PUBLIC)
	$(RANLIB) $(PUBLIC)

$(EXAMPLE):	$(EXAMPLE).o
	$(CC) $(LDFLAGS) -o $(EXAMPLE) $(EXAMPLE).o $(LDLIBS)

.c.o:
	$(CC) $(CFLAGS) $(CINCS) -c $<

########################################################################
#	miscellaneous targets
########################################################################

get-manpages:	$(MANPAGES)

get-examples:	$(EGSOURCES)

get-sources:	$(SOURCES)

get-headers:	$(HEADERS)

########################################################################
#	Anything after this line is added by ``make depend''
#	and is not part of the production ``Makefile''. If you
#	are editing something past this message, then you are
#	probably editing the wrong file.
########################################################################

