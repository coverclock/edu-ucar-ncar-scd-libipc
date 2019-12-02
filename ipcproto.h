/*
**	I P C P R O T O
**
**	Copyright 1994 University Corporation for Atmospheric Research
**			All Rights Reserved
**
**	@(#)ipcproto.h	4.1 94/05/16 jloan@ncar.ucar.edu
**
**	This is a little macro that simplifies prototyping function
**	declarations for porting between ANSI C and non-ANSI C
**	compilers.
*/

#ifndef H_IPCPROTO
#define H_IPCPROTO

#ifdef PROTOTYPE_LIBIPC
#define IPCPROTO(list)	list
#else /* PROTOTYPE_LIBIPC */
#define IPCPROTO(list)	()
#endif /* PROTOTYPE_LIBIPC */

#endif /* !H_IPCPROTO */
