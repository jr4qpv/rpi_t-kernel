/*
 *----------------------------------------------------------------------
 *
 *    T-Kernel Software Library
 *
 *----------------------------------------------------------------------
 */

/*
 *	@(#)cmd_loca.h (t2ex) 2016/11/17
 */
#ifndef __CMD_LOCAL_H__
#define __CMD_LOCAL_H__

#ifdef	USE_T2EX_FS
  #include <stdio.h>

  #define	P			printf
  #define	Gets(buf, bufsz)	fgets(buf, bufsz, stdin)
#else
 #ifdef	USE_MISC_CPRINT
  #define	P			cprintf
  #define	Gets(buf, bufsz)	cgetstring((char*)buf, bufsz)
 #else
  #define	P			tm_printf
  #define	Gets(buf, bufsz)	tm_getline(buf)
 #endif
#endif


#define		CMD_PROMPT			"T2EX >> "
//#define		CMD_PROMPT			"CCP> "

#define		FS_CMD_ENABLE		0



#endif  /* __CMD_LOCAL_H__ */


/*----------------------------------------------------------------------
#|History of "cmd_local.h"
#|========================
#|* 2016/12/07	New created.(By T.Yokobayashi)
#|
*/
