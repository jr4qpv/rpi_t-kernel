/*
 *----------------------------------------------------------------------
 *    T-Kernel Software Library
 *
 *    Copyright(C) 2016 by T.Yokobayashi.
 *----------------------------------------------------------------------
 */

/*
 *	@(#)main.c (test) 2016/02/26
 *  テストサンプルメインプログラム
 */

#include <basic.h>
///#include <tm/tmonitor.h>
#include <misc/libmisc.h>

EXPORT int main(int ac, char* av)
{
	if (ac >= 0) {
		cprintf("Hello, system program\n");
	}
	else {
		cprintf("Good bye, system program\n");
	}

	return 0;
}


/*----------------------------------------------------------------------*/
#if 0
#|【main.c 変更履歴】
#|□2016/02/26	新規作成(by T.Yokobayashi)
#|
#endif
