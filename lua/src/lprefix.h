/*
** $Id: lprefix.h,v 1.2 2014/12/29 16:54:13 roberto Exp $
** Definitions for Lua code that must come before any other header file
** See Copyright Notice in lua.h
*/

#ifndef lprefix_h
#define lprefix_h

#ifdef __GNUC__		////// kari //////
//#include <basic.h>
#define	wint_t		int
#define	l_signalT	int
int errno;
#define	EXIT_SUCCESS		E_OK
#define	EXIT_FAILURE		-1
#define	CLOCKS_PER_SEC		1000
#define	clock_t				long

/* ISO C handling with long jumps(See ldo.c) */
#define LUAI_THROW(L,c)		/* dummy */
#define LUAI_TRY(L,c,a)		/* dummy */
#define luai_jmpbuf		int  /* dummy variable */

/* See luaconf.h */
#define lua_getlocaledecpoint()		0

#define strerror(no)		"errormsg"

/* See liolib.c */
#define	tmpfile()			"tmpfile"

/* See lmathlib.c */
#define l_rand()	0
#define l_srand(x)	0
#define L_RANDMAX	2147483647	/* (2^31 - 1), following POSIX */

/* See loslib.c */
#define system(cmd)		0
#define remove(fn)		0
#define rename(f1,f2)	0
#define LUA_TMPNAMBUFSIZE	32
#define lua_tmpnam(b,e) { strcpy(b,"tmpnam"); }
#define getenv(s)		""
#define clock()			0
#define l_gmtime(t,r)	0
#define l_localtime(t,r)  0
//#define setlocale(a,l)	NULL
#define exit(no)		/* */


#endif	//////////////////


/*
** Allows POSIX/XSI stuff
*/
#if !defined(LUA_USE_C89)	/* { */

#if !defined(_XOPEN_SOURCE)
#define _XOPEN_SOURCE           600
#elif _XOPEN_SOURCE == 0
#undef _XOPEN_SOURCE  /* use -D_XOPEN_SOURCE=0 to undefine it */
#endif

/*
** Allows manipulation of large files in gcc and some other compilers
*/
#if !defined(LUA_32BITS) && !defined(_FILE_OFFSET_BITS)
#define _LARGEFILE_SOURCE       1
#define _FILE_OFFSET_BITS       64
#endif

#endif				/* } */


/*
** Windows stuff
*/
#if defined(_WIN32) 	/* { */

#if !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS  /* avoid warnings about ISO C functions */
#endif

#endif			/* } */

#endif

