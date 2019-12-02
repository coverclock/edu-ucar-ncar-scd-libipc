/*
**	@(#)egipcaddresses.c	1.3 95/08/06 jsloan@ncar.ucar.edu
*/

#include "ipc.h"

main(int argc, char **argv)
	{
	IPCADDRESS addr;
	int ndx;
	char name[64];

	if (argc<2)
		exit(1);

	for (ndx=0; ; ndx++)
		if ((addr=ipcaddresses(argv[1],ndx))==0L)
			break;
		else
			printf("%s: %s[%d]=%s=0x%lx=%s\n",
				argv[0],argv[1],ndx,ipcformat(addr,0,name),addr,
				ipclocal(addr)?"local":"remote");

	exit(0);
	}
