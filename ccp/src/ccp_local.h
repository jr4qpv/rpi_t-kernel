/*
 *----------------------------------------------------------------------
 *
 *    T-Kernel Software Library
 *
 *----------------------------------------------------------------------
 */

/*
 *	@(#)ccp_loca.h (appl) 2016/11/17
 */
#ifndef __CCP_LOCAL_H__
#define __CCP_LOCAL_H__

#define	USE_APP_EXTCMD		1
#define	USE_PRINTF_MODE		2		/* =0:tm_printf, =1:printf, =2:cprintf */

#if USE_PRINTF_MODE == 2
 #define	P					cprintf
 #define	Gets(buf, bufsz)	cgetstring(buf, bufsz)
#elif USE_PRINTF_MODE == 1
 #include <stdio.h>
 #define	P					printf
 #define	Gets(buf, bufsz)	fgets(buf, bufsz, stdin)
#else
 #define	P					tm_printf
 #define	Gets(buf, bufsz)	tm_getline(buf)
#endif


#define		CCP_PROMPT			"T2EX >> "
//#define		CCP_PROMPT			"CCP> "



#endif  /* __CCP_LOCAL_H__ */


/*----------------------------------------------------------------------
#|History of "ccp_local.h"
#|========================
#|* 2016/11/17	New created.(By T.Yokobayashi)
#|
*/
