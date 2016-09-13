/*
 *----------------------------------------------------------------------
 *    T-Kernel Software Library
 *
 *    Copyright(C) 2016 by T.Yokobayashi.
 *----------------------------------------------------------------------
 */

/*
 *	@(#)main.c (sample) 2016/02/25
 *  テストサンプルメインプログラム
 */

#include <basic.h>
#include <tm/tmonitor.h>

EXPORT int main(int ac, char* av)
{
	if (ac >= 0) {
		tm_putstring("Hello, system program\n");
	}
	else {
		tm_putstring("Good bye, system program\n");
	}

	return 0;
}


/*----------------------------------------------------------------------*/
#if 0
#|【main.c 変更履歴】
#|□2016/02/25	新規作成(by T.Yokobayashi)
#|
#endif
