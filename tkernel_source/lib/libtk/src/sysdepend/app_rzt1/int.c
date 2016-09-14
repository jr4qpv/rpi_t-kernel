/*
 *----------------------------------------------------------------------
 *    T-Kernel 2.0 Software Package
 *
 *    Copyright 2011 by Ken Sakamura.
 *    This software is distributed under the latest version of T-License 2.x.
 *----------------------------------------------------------------------
 *
 *    Released by T-Engine Forum(http://www.t-engine.org/) at 2011/05/17.
 *    Modified by TRON Forum(http://www.tron.org/) at 2015/06/01.
 *
 *----------------------------------------------------------------------
 *
 *    Modified by T.Yokobayashi at 2016/07/06.
 *
 *----------------------------------------------------------------------
 */

/*
 *	@(#)int.c (RZT1) 2016/07/17
 *
 *	RZT1 interrupt controller management
 *
 *	intvec passed to the functions for interrupt controller must
 *      be within the valid ranges of IRQ and GPIO interrupt.
 *	If a value outside the valid range is passed, the subsequent
 *      correct behavior of the system is not guaranteed.
 */

#include <basic.h>
#include <tk/syslib.h>
#include <tk/sysdef.h>
#include <misc/renesas/rzt1.h>


#define	BITNO(intvec)	( (intvec) % 32 )		/* bit number */
#define	AVICNO(intvec)	( (intvec) / 32 )			/* INT0～INT255 reg offset */
#define	BVICNO(intvec)	( ((intvec)-256) / 32 )		/* INT256～ reg offset */



/* ------------------------------------------------------------------------ */

/*
 * Disable / Enable interrupt
 */
Inline UW _disint( void )
{
	UW	imask;
	Asm("	mrs	%0, cpsr	\n"
	"	orr	ip, %0, %1	\n"
	"	msr	cpsr_xc, ip	"
		: "=r"(imask)
		: "i"(PSR_DI)
		: "ip" );
	return imask;
}
Inline void _enaint( UW imask )
{
	Asm("msr cpsr_xc, %0":: "r"(imask));
}
#define	_DI(imask)	( imask = _disint() )
#define	_EI(imask)	( _enaint(imask) )

/*
 * Set interrupt mode.
 *	Set the interrupt mode specified by `intvec' to the mode given
 *  	by `mode'. If an illegal mode is given, subsequent correct behavior
 *      of the system is not guaranteed.
 *
 *	The case of IRQ
 *	mode := IM_ENA | IM_INV
 *	or	IM_DIS
 *
 *	The case of GPIO
 *	mode := IM_ENA | IM_LEVEL | (IM_HI || IM_LOW) | IM_ASYN
 *	 or IM_ENA | IM_EDGE | (IM_HI || IM_LOW || IM_BOTH) | IM_ASYN
 *	 or IM_DIS
 *
 *	If IM_ENA is specified, the mode setting is done, and
 *      interrupt is disabled (DisableInt) and the interrupt pin is
 *      asserted.
 *      If IM_DIS is specified, interrupt pin is disasserted.
 *      disasserted interrupt pin doesn't generate interrupt even if
 *      it is enabled (EnabledInt).
 *      In the initial state, pins are dis-asserted (IM_DIS).
 *
 *	The initial status of the following interrupt controllers
 *	that manage GPIO interrupt is such that
 *      as if IM_ENA had been specified and interrupts are enabled
 * 	(EnableInt).
 *
 *		IRQ26	GIO6 Interrupt (GPIO port  96--111)
 *		IRQ27	GIO7 Interrupt (GPIO port 112--127)
 *		IRQ50	GIO0 Interrupt (GPIO port   0-- 15)
 *		IRQ51	GIO1 Interrupt (GPIO port  16-- 31)
 *		IRQ52	GIO2 Interrupt (GPIO port  32-- 47)
 *		IRQ53	GIO3 Interrupt (GPIO port  48-- 63)
 *		IRQ79	GIO4 Interrupt (GPIO port  64-- 79)
 *		IRQ80	GIO5 Interrupt (GPIO port  80-- 95)
 */
EXPORT void SetIntMode( INTVEC intvec, UINT mode )
{
#if 1
	UW	m, n;
	_UW	*p;

	if ((intvec < 1) || (intvec > 300))
		return;							// ベクタ番号が不正の時は無視

	/* エッジ割り込みモードの設定（IM_LEVEL/IM_EDGE） */
	m = 1 << BITNO(intvec);				// ビット位置

	if (intvec < 256) {
		n = AVICNO(intvec);				// オフセット
		p = &VIC.PLS0.LONG;				// レジスタ先頭アドレス
	}
	else {
		n = BVICNO(intvec);				// オフセット
		p = &VIC.PLS8.LONG;				// レジスタ先頭アドレス
	}

	if (mode & IM_LEVEL)
		p[n] &= ~m;						// =0:レベル検出割り込み(IM_LEVEL)
	else
		p[n] |= m;						// =1:エッジ検出割り込み(IM_EDGE)


#else
	UW	m = 1 << BITNO(intvec);
	UINT	imask;

	if ( intvec < IV_GPIO(0) ) {
		/* IRQ */
		const struct aint_reg	*r = AINT + AINTNO(intvec);

		out_w(r->IT0_IDS,  m);	/* disable interrupt */
		out_w(r->IT0_IDSS, m);	/* disassert interrupt pin */
		if ( (mode & IM_ENA) == 0 ) return;

		if ( (mode & IM_INV) == 0 ) out_w(r->IT_PINV_CLR, m);
		else			    out_w(r->IT_PINV_SET, m);

		out_w(r->IT0_IENS, m);	/* assert interrupt pin */
	} else {
		/* GPIO */
		UINT	gb = GIOB[GIONO(intvec)];
		UINT	n;
		UW	mm, mv;

		_DI(imask);
		out_w(GIO_IDS(gb), m);			    /* disable interrupt */
		out_w(GIO_IIA(gb), in_w(GIO_IIA(gb)) & ~m); /* disassert interrupt pin */
		_EI(imask);
		if ( (mode & IM_ENA) == 0 ) return;

		n = (intvec - IV_GPIO(0)) % 8 * 4;
		mm = 0xf << n;
		mv = ((mode >> 8) & 0xf) << n;
		n = (intvec - IV_GPIO(0)) % 32 / 8;
		_DI(imask);
		out_w(GIO_IDT(n, gb), (in_w(GIO_IDT(n, gb)) & ~mm) | mv);
		out_w(GIO_IIA(gb), in_w(GIO_IIA(gb)) | m); /* assert interrupt pin */
		_EI(imask);
	}
#endif
}

/*
 * Enable interrupt
 *	enable interrupt specified by `intvec'
 */
EXPORT void EnableInt( INTVEC intvec, INT level )
{
	UW	m, n;
	_UW	*p;

	if ((intvec < 1) || (intvec > 300))
		return;							// ベクタ番号が不正の時は無視

	level = level & 0x0f;				// 下位4bitのみ有効
	m = 1 << BITNO(intvec);				// ビット位置

	if (intvec < 256) {
		n = AVICNO(intvec);				// オフセット
		p = &VIC.PRL1.LONG;				// レジスタ先頭アドレス
		p[n-1] = level;					// 割り込み優先レベル

		p = &VIC.IEN0.LONG;				// レジスタ先頭アドレス
	}
	else {
		n = BVICNO(intvec);				// オフセット
		p = &VIC.PRL256.LONG;			// レジスタ先頭アドレス
		p[n] = level;					// 割り込み優先レベル

		p = &VIC.IEN8.LONG;				// レジスタ先頭アドレス
	}

	p[n] = m;							// Interrupt Enable
}

/*
 * Disable interrupt
 *	disable interrupt specified by `intvec'
 */
EXPORT void DisableInt( INTVEC intvec )
{
	UW	m, n;
	_UW	*p;

	if ((intvec < 1) || (intvec > 300))
		return;							// ベクタ番号が不正の時は無視

	m = 1 << BITNO(intvec);				// ビット位置

	if (intvec < 256) {
		n = AVICNO(intvec);				// オフセット
		p = &VIC.IEC0.LONG;				// レジスタ先頭アドレス
	}
	else {
		n = BVICNO(intvec);				// オフセット
		p = &VIC.IEC8.LONG;				// レジスタ先頭アドレス
	}

	p[n] = m;							// Interrupt Disable
}

/*
 * Clear the request for interrupt
 *	clear the request of interrupt specified by `intvec'.
 *	We need to clear the requests only in the case of edge-trigger interrupts.
 */
EXPORT void ClearInt( INTVEC intvec )
{
	UW	m, n;
	_UW	*p;

	if ((intvec < 1) || (intvec > 300))
		return;							// ベクタ番号が不正の時は無視

	m = 1 << BITNO(intvec);				// ビット位置

	if (intvec < 256) {
		n = AVICNO(intvec);				// オフセット
		p = &VIC.PIC0.LONG;				// レジスタ先頭アドレス
	}
	else {
		n = BVICNO(intvec);				// オフセット
		p = &VIC.PIC8.LONG;				// レジスタ先頭アドレス
	}

	p[n] = m;							// エッジ検出ビットクリア
}

/*
 * Check the existence of interrupt request
 *	check the existence of request for interrupt specified by intvec
 *	If there is, TRUE (non-zero value) is returned
 *
 *	The existence of the request of interrupt is checked by
 *      'raw' status register.
 */
EXPORT BOOL CheckInt( INTVEC intvec )
{
#if 1


	return 0;
#else
	UW	sts;

	if ( intvec < IV_GPIO(0) ) {
		/* IRQ */
		const struct aint_reg	*r = AINT + AINTNO(intvec);
		sts = in_w(r->IT0_RAW);
	} else {
		/* GPIO */
		UINT	gb = GIOB[GIONO(intvec)];
		sts = in_w(GIO_RAW(gb));
	}

	return (sts >> BITNO(intvec)) & 1;
#endif
}


/*
 * End of interrupt
 *  intvec: don't care
 */
EXPORT void EndOfInt( INTVEC intvec )
{
	/* Dummy write */
	VIC.HVA0.LONG = 0x00000000L;		/* 割り込み処理完了 */
}


/*----------------------------------------------------------------------
#|History of "int.c"
#|=======================
#|* 2016/07/06	[app_rzt1]用に、[tef_em1d]用の"int.c"を参考に作成。
#|* 2016/07/14	EnableInt()の第2引数は割り込みレベルとする。
#|* 2016/07/14	EndOfInt()を定義し実装した。
#|
*/
