/*
	databox.c	�ǡ����ܥå�������ѥ���

	Copyright (C) 1989-2012 by Personal Media Corporation
*/

#if	0
/*======================================================================

�� ư����ˡ��

	databox��-b]��-n] [-l] [-s[a]] ���ϥե�����̾ [���ϥե�����̾]

		-b : �ӥå�����ǥ��������
		-n : �ǡ����ܥå��������Ǥʤ����إå���ʬ�ʤ��ǡ�ñ�ʤ�Х��ʥ�
		     �ǡ������������뤿��˻��Ѥ��롣
		-l : �ꥹ�ƥ��󥰽��� (*.lst)
		-s : ����������
		-a : ���������ϤΥ��饤���Ȥ� long �ˤ���

	 -- ���ϥե�����̾��ά����ɸ�����
	 -- �ꥹ�ƥ��󥰥ե�����̾�ϡ�(���ϥե�����̾).lst
	 -- ���餫�Υ��顼������ȥ�å�������ɽ�����������Ǽ¹Ԥ���ߤ��롣

�� ���ϥե��������:

	���Τη���:
		<�ǡ��������׻���>
		<�ǡ����ֹ����>
		<�ǡ�������>

		<�ǡ����ֹ����>
		<�ǡ�������>
		    : :

		�嵭�η����֤���

		-n ���ץ�������ξ���<�ǡ��������ΤߤȤʤ롣

	������:
		'--' �ʹߤ�ʸ���Ϲ����ޤǥ����ȤȤߤʤ����ɤ����Ф���

	���ڤ�ʸ��:
		��������ʸ��(���ԡ����ִޤ�)��','�� ';'��'('��')'��'{'��'}'
		�Ϥ��٤ƶ���ȸ��ʤ���롣
		�Ԥζ��ڤ�ϥ����Ȥν�λ�ʳ������̤ΰ�̣�ϻ����ʤ���

	�ǡ��������׻���:
		% �θ�˥ǡ��������פ�(���)�ǻ��ꤹ�롣
		���θ��°������ꤹ�롣
		-n ���ץ�������ξ��ϥǡ��������׻���Ϥ��äƤϤ����ʤ���

		��:	% 1  0

	�ǡ����ֹ����:
		# �θ�ˡ��ǡ����ֹ桢�桼���������(���)�ǻ��ꤹ�롣
		-n ���ץ�������ξ��ϥǡ����ֹ����Ϥ��äƤϤ����ʤ���

		��:	# 1 0 0

	�ǡ�������:
		# �����ľ�夫�顢���� % �ޤ��� # �ޤǥǡ����Ȥߤʤ���

	�ǡ�������:
		�ǡ������ܤȤ��Ƥϡ�ʸ����ȼ������롣

	TRON ʸ����:
		" �ǰϤޤ줿 EUC ����ʸ����ǡ�TRON �����ɤ��Ѵ�����롣

		�Ǹ�� " ��ľ��˿�����³�����ϡ����ο�����ʸ�����򼨤���
		ʸ�����������ʤ����ϣ����ѥåɤ���롣

		���������ץ�����:
			\n, \f, \t, \r, \b	���楳����(��̥Х��Ȥ� 0)
			\(8 �ʿ� 3 ��ޤ�)	Ǥ�դΥ�����
			\x(16 �ʿ� 4 ��ޤ�)	Ǥ�դΥ�����

		Ⱦ��ʸ��(�ѿ���)�ξ��ϡ���̥Х��Ȥ� 0 ���ѥåɤ��졢
		WARNING ���Ǥ롣

		��:	"����ʸ����\0"
			"�޷�"16	-- "�޷�"�θ�� 14 �Ĥ� 0 ������

	ASCII ʸ����:
		' �ǰϤޤ줿 ASCII ʸ����EUC ����ʸ�����
		���ΤޤޤȤʤꡢ����ǥ�������Ѵ��ϹԤ��ʤ���
		���ʸ�����ξ��ϡ��Ǹ�˥Х��� 0 ���ѥåɤ���롣

		�Ǹ�� ' ��ľ��˿�����³�����ϡ����ο�����ʸ���� / 2 �򼨤�
		ʸ�����������ʤ����� 0  ���ѥåɤ���롣

		���������ץ�����:
			\n, \f, \t, \r, \b	���楳����
			\(8 �ʿ� 3 ��ޤ�)	Ǥ�դΥ�����
			\x(16 �ʿ� 2 ��ޤ�)	Ǥ�դΥ�����

		��:	'ASCII'		-- �Ǹ�˥Х��� 0 ���ѥåɤ���롣
			'TEXT'16	-- 'TEXT'�θ�� 28 �ĤΥХ��� 0 ������

	���᡼���ǡ���:
		` �ǰϤޤ줿�������¤Ӥϥ��᡼���ǡ������Ȥ��ƽ�������롣

		��:	`��������������������������������`	-- 0x03 0x00

	CBMP:
	CBMPIMG:
		$CBMP(compac planes pixbits rowbyte rect data) �� CBMP ������
		�ǡ������������롣
		data �� ���� ���¤Ӥǻ��ꤹ�롣���Ϲ��ˡ���������Ѵ�����롣

	    ��	planes == 1, pixbits == 0x808, comapc == 0 �ʳ��Υǡ����Ͻ���
		�Ǥ��ʤ���

	    ��	$CBMP ������ $CBMPIMG �Ȥ���ȡ��ӥåȥޥåפΥ��᡼���ǡ���
		�Τߤ��������� (BMP ��¤�μ��Ȥ���������ʤ�)

	��:
		�ǡ������ǡ��ޤ���Ǥ�ոĤΥǡ������Ǥ򥪥ڥ졼����Ϣ�뤷�����

		���ڥ졼���ϡ�'+' '-'  '*'  '/' �ǡ���������֤˱黻����롣
		����˶������ζ��ڤ�ʸ�������äƤϤ����ʤ���

		���Υǡ���Ĺ�ϡ�BYTE(8bit)��WORD(16bit)��LONG(32bit)�Τ����줫
		�ǡ����η�̤ϼ���Υǡ������ǤǺǤ�Ĺ���ǡ���Ĺ�ˤʤ롣

		��:	0x100H+0x10Y	-- 0x110 (WORD)
			128/2L		-- 64	 (LONG)

	�ǡ�������:
		<����>		BYTE/WORD/LONG (BASE �˽���)
		<����>Y		BYTE( 8�ӥå�)
		<����>H		WORD(16�ӥå�)
		<����>L		LONG(32�ӥå�)

		���ͤϡ��ʲ���ɽ������ǽ
		<10 �ʿ�����>		10 �ʿ�
		0x<16 �ʿ�����>		16 �ʿ�
		0<8 �ʿ�����>		8 �ʿ�

		<����ܥ�>		����ܥ���(BYTE/WORD/LONG)
					BYTE/WORD/LONG �ϥ���ܥ������ͤˤ��
		<����ܥ�>:Y		����ܥ���(BYTE)
		<����ܥ�>:H		����ܥ���(WORD)
		<����ܥ�>:L		����ܥ���(LONG)
		( <����ܥ�>:W	����ܥ���(WORD) )

	����ܥ����:
		�ѻ�('_'��ޤ�)�ǻϤޤ���� 16 ʸ���ޤǤαѿ����Υ���ܥ�
		���� 1000 �ġ�

		.<����ܥ�> = <��>
			����ܥ���ͤ�<��>���ͤȤ��롣
			����ܥ�Υǡ���Ĺ��<��>�Υǡ���Ĺ�Ȥʤ롣

		.<����ܥ�>:
			���ߤΥǡ������ե��åȤ򥷥�ܥ���ͤȤ��롣
			����ܥ�Υǡ���Ĺ�����ꤵ��ʤ�(����)��
			���Υ���ܥ�򻲾Ȥ����Ȥ��� BASE ������ˤ��ǡ���Ĺ
			�����ꤵ��롣

			�ǡ������ե��åȤϡ��ġ��Υǡ����������Ƭ�� 0 �Ȥ���
			�Х��ȥ��ե��åȤȤʤ롣

		����ܥ�ˤϥ����Х륷��ܥ�ȡ������륷��ܥ뤬���롣

		�����Х륷��ܥ�:
			�ǽ�Υǡ����ֹ��������������������ܥ�ǡ�
			���Τˤ錄�ä�ͭ����

		�����륷��ܥ�:
			�ǡ����ֹ����θ�������������ܥ�ǡ����Υǡ����ֹ�
			�Υǡ�����ǤΤ�ͭ����

		����ܥ���ͤκ�����ϲ�ǽ�Ǥ��뤬��WARNING ���Ǥ롣
		..<����ܥ�> �Ȥ������ϡ�WARNING �ϤǤʤ���

		��:
			.Global = 0x1111L
			% 1 0
			# 1 0 0
			Global		-- 0x1111 0x0000
			Global:W-1	-- 0x1110
			.offset:
			offset		-- 0x0006
			#2 0 0
			offset:L	-- 0x0004 0x0000
			.offset:

		����ܥ���ͤ��������Ȥϲ�ǽ�Ǥ��뤬������ܥ뤬
		�ޤ�̤����ξ�硢�ǡ���Ĺ�� BASE ������ˤ������ä�Ĺ���ȸ���
		�����ᡢ����ܥ���ͤΥǡ���Ĺ���ۤʤ�Ȥ��ˤϡ�����Ū��ɽ����
		��ɬ�פ����롣

		��:
			A
			undef
			.A:
			.undef = 10L

			 �ξ�硢A ���ͤ������ˤʤ�Τǡ�
			�ʲ��ε��ҤȤ���ɬ�פ����롣

			A:
			undef:L    -- �ޤ��� undef+0L �ʤ�
			.A:
			.undef = 10L


	�����ƥॷ��ܥ�:

		BASE	-- ���ͤδ���򼨤���
			   ����ͤ� 0�� 0 �ʳ������ꤹ��ȡ�������Ϥ����ͤ�
			   ����Ȥ��Ʋ�ᤵ��롣
			   LONG ���ͤ����ꤹ��ȡ�LONG �ο����Ȥʤ롣Ʊ�ͤˡ�
			   BYTE ���ͤ����ꤹ��� BYTE �ο��͡�WORD ���ͤ�����
			   ����� WORD �ο��ͤȤʤ롣�ǥե���Ȥ� WORD��
			   ��:
				.BASE = 16
				0ffff 0aaaa 5555	-- 16 �ʿ�(WORD)
				.BASE = 0
				5555			-- 10 �ʿ�(WORD)
				.BASE = 16L
				0ffff 0aaaa 5555	-- 16 �ʿ�(LONG)
				.BASE = 0L
				5555			-- 10 �ʿ�(LONG)

		OFFSET	-- ���λ����Υ��ե��åȤ򼨤���
			   ���Ȼ��� BASE ������ˤ��ǡ���Ĺ�����ꤵ��롣
			   ��:
				OFFSET+2	-- "ʸ��"�ؤΥ��ե��å�(WORD)
				"ʸ��"

				OFFSET+4L	-- "ʸ��"�ؤΥ��ե��å�(LONG)
				"ʸ��"

		ALIGN	-- ���饤����Ȥ���ꤹ�롣����.ALIGN �����ꤵ��
			   ��ޤ�#???��.????:�ˤ�륪�ե��åȤϡ����ꤷ����
			   �饤����Ȥˤ��碌���롣
			��:
				[00]�ϥѥǥ���
							.ALIGN = 1
				0000	01		.p01:	1Y
				0001	01		.p02:	1Y
							.ALIGN = 2
				0002	01 [00]		.p03:	1Y
				0004	01 [00 00 00]	.p04:	1Y
							.ALIGN = 4
				0008	01		.p05:	1Y
							.ALIGN = 1
				0009	01		.p06:	1Y

		ɸ��ǡ��������׻���(WORD)

			PTR_DATA	1
			PICT_DATA	2
			PAT_DATA	3
			BMAP_DATA	4
			FIG_DATA	5
			TEXT_DATA	6
			PARTS_DATA	7
			MENU_DATA	8
			GMENU_DATA	9
			PANEL_DATA	10
			USER_DATA	64

�� ���ϥե��������:

	 - - - - - -+-----------------------+
		    |	�إå� ( 16 �Х���) |
	 ��	    |- - - - - - - - - - - -|
	 ��	+---|	����ǥå���  1     |		N: �ǡ������ܿ�
	 ��	|   |   (10 + 16 N �Х���)  |---+-+
	 ��	+-->|- - - - - - - - - - - -|	| |
	 ��	+---|	����ǥå���  2     |	| |
	 ��	|   |			    |	| |
	 ��	+-->|- - - - - - - - - - - -|	| |
		    |			    |	| |
		   //			    //	| |
		    |			    |	| |
	 - - - - - -+-----------------------+	| |
		    |	�ǡ������� A	    |<--+ |
		    |			    |	  |
	 ��	    |- - - - - - - - - - - -|	  |
	 ��	    |	�ǡ������� B	    |<----+
	 ��	    |			    |  : :
	 ��	    |- - - - - - - - - - - -|
		    |			    |
		   //			    //
		    |			    |
	 - - - - - -+-----------------------+

�� �ꥹ�ƥ��󥰷���:

	��Ƭ�˥���ǥå��������⤬��16 �ʿ��ǽ��Ϥ���롣
	�ǡ������ϡ��������Ԥ�ľ����б�����ǡ����� 16 �ʿ��ǽ��Ϥ���롣
	�ǡ������Υ��ɥ쥹�ϡ��ǡ���������Ƭ�� 0 �Ȥ������ɥ쥹�ǡ�
	����ǥå������Ϥդ��ޤ�ʤ���

======================================================================*/
#endif

/* �����Ķ��Υإå� */
#include <stdio.h>
#include <ctype.h>

/* BTRON ���ͤη���� */
typedef char		B;	/* ����դ�  8�ӥå����� */
typedef short		H;	/* ����դ� 16�ӥå����� */
typedef int		W;	/* ����դ� 32�ӥå����� */
typedef unsigned char	UB;	/* ���̵��  8�ӥå����� */
typedef unsigned short  UH;	/* ���̵�� 16�ӥå����� */
typedef unsigned int	UW;	/* ���̵�� 32�ӥå����� */

typedef	unsigned char	BYTE;
typedef	short		WORD;
typedef	unsigned short	UWORD;
typedef	int		LONG;
typedef	unsigned int	ULONG;
typedef	unsigned short	TCODE;

#define	FAST		register

typedef struct {
	H	ntyp;		/* �ܥå�����Υǡ��������� ID �ο� */
	H	ixsize;		/* ����ǥå���������Х��ȿ� */
	H	resv[6];
} DBOX_HDR;

typedef struct {
	W	id;		/* �ǡ����ֹ� */
	W	pos;		/* �б�����ǡ������ΤؤΥ��ե��å� */
	W	size;		/* �ǡ������ΤΥХ��ȥ����� */
	W	info[2];	/* ���������ΰ� */
} DNUM_INX;

typedef struct {
	W	npos;		/* ���Υ����פΥ���ǥå����ؤΥ��ե��å� */
	W	typ;		/* �ǡ��������� ID */
	UW	attr;		/* �ǡ�����°�� */
	W	nd;		/* ���Υ����פ�°����ǡ������ܤο� */
	DNUM_INX dix[1];	/* ���ܥ���ǥå������� (nd �Ĥ�����) */
} DTYP_INX;

#define	INDEXSZ		1024 * 20


FILE	*infile, *oufile, *tmpf, *lstfile;

WORD	bigend = 0;
WORD	cpuend = 0;
WORD	noheader = 0;
WORD	list = 0;
WORD	source = 0;
LONG	lstaddr = 0L;
WORD	ch;
LONG	dcnt, tycnt, idno, lineno;
BYTE	 *dps;
DTYP_INX *dp;
BYTE	ofn[128];
BYTE	ifn[128];
#define	BUF_SIZE	1024
BYTE	buf[BUF_SIZE];
BYTE	*bufp;

/* symbol table */
#define	MAX_SYM		2000
WORD	max_sym, pass2;
WORD	sym_level = 0;
LONG	id_base = 0L;

WORD	system_symbols;

typedef enum LV {
	LV_UNDEF	= 0,
	LV_BYTE		= 1,
	LV_WORD		= 2,
	LV_LONG		= 3
} LV;

struct {
	BYTE	nam[16];
	LONG	val;
	LV	lv;
	WORD	level;
} symbol[MAX_SYM];

typedef struct {
	LONG	val;
	LV	lv;
} DmpBuf;

DmpBuf	dbuf[BUF_SIZE * 4];
DmpBuf	*dbufp;

#define	TO_WORD(x,y)	((x << 8) | y) & 0x7f7f
#define	PAD_WORD(x)	x & 0xff
#define	U_BYTE(x)	(x >> 8) & 0xff
#define	L_BYTE(x)	x & 0xff
#define	CWORD(x)	(bigend ? x : ((x & 0xff)<<8)|((x>>8) & 0xff))
#define CLONG(x)	(bigend ? x : ((x & 0xff)<<24)|((x & 0xff00)<<8)|((x & 0xff0000)>>8)|((x >>24) & 0xff))

#define	TO_XWORD(x,y)	(bigend ? (x<<8|y):(y<<8|x))

#define	XCWORD(x)	(bigend == cpuend ? x :((x & 0xff)<<8)|((x>>8) & 0xff))
#define XCLONG(x)	(bigend == cpuend ? x :((x & 0xff)<<24)|((x & 0xff00)<<8)|((x & 0xff0000)>>8)|((x >>24) & 0xff))

#define ISSP(c) 	(c == ' ')

#ifdef	WIN			/* for MS-DOS or Win */
#define	KANJI(c)	((c >= 0x80 && c < 0xa0) || c >= 0xe0)
#define	HANKANA(c)	(c >= 0xa0 && c < 0xe0)
UH	sjistojis (UH sj);
#define	chgtojis(x)	(sjistojis(x))
#else				/* for Sun */
#define	KANJI(c)	(c & 0x80)
#define	HANKANA(c)	(c == 250)
#define	chgtojis(x)	(x & 0x7f7f)
#endif

/*======================================================================*/
main(ac,av)
/*======================================================================*/
WORD	ac;
BYTE	**av;
{
	FAST BYTE	*cp;
	FAST WORD	i;
	union	{
		BYTE	b[2];
		WORD	w;
	} chk;

	/* CPU ����ǥ�����Υ����å� */
	chk.w = 0xAA55;
	cpuend = (chk.b[0] == 0x55) ? 0 : 1;	/* 0: little, 1: big	*/

	/* get option switches */
	while (--ac && *(cp = (*++av)) == '-' ) 
	    while (*++cp)
		switch (*cp) {
		    case 'b': case 'B': 	/* big endian */
			bigend = 1;	break;
		    case 'n': case 'N': 	/* no header */
			noheader = 1;	break;
		    case 'l': case 'L': 	/* listing */
			list = 1;	break;
		    case 's': case 'S': 	/* source */
			source = 2;	break;
		    case 'a': case 'A': 	/* source alignment */
			source = 4;	break;
		    default :
			usage();
		}

	
	if (ac < 1 || ac > 2) usage();
	strcpy (ifn, cp);
#ifdef	WIN
	if ((infile = fopen((char *)ifn,"r")) == NULL)
					eexit("Input file not found");
#else
	if ((infile = fopen(ifn,"r")) == NULL)
					eexit("Input file not found");
#endif
	cp = *++av;
	if (ac <= 1) {oufile = stdout; ofn[0] = 0;}
	else {
	    strcpy (ofn,cp);
#ifdef	WIN
	    oufile = fopen((char *)ofn, source == 0 ? "wb" : "w");
#else
	    oufile = fopen(ofn, "w");
#endif
	    if (oufile == NULL) eexit ("Output file cant open");
	}
	tmpf = tmpfile();			/* open temporary file */

	if (list) {
	    for (i = strlen(ifn); --i > 0;)
			if (ifn[i] == '.') {ifn[i] = 0; break;}
	    strcat (ifn, ".lst");
#ifdef	WIN
	    if ((lstfile = fopen((char *)ifn,"w")) == NULL)
					eexit ("Listing file cant open");
#else
	    if ((lstfile = fopen(ifn,"w")) == NULL)
					eexit ("Listing file cant open");
#endif
	}

	make_data();				/* make data file */

	fclose(infile);
	fclose(oufile);

	return 0;
}
/*----------------------------------------------------------------------*/
WORD get_ch()					/* get 1 character	*/
/*----------------------------------------------------------------------*/
{
	FAST WORD	c;

	c = fgetc (infile);
	if (pass2 && lstfile && c != EOF) fputc (c, lstfile);
	return (c);
}
/*----------------------------------------------------------------------*/
WORD in_ch(flg)					/* get 1 character	*/
/*----------------------------------------------------------------------*/
int	flg;
{
	/* ignore chars after COMMENT till NL */

static	WORD	unget_ch = -1;
static	WORD	nlflg = 0;
static	WORD	kanji2 = 0;
	LONG	pos;
	DmpBuf	*p;
	char	*fmt;

	if (nlflg) {
	   if (pass2 && lstfile && (dbufp != dbuf)) {
		fprintf (lstfile, "=%04x: ", lstaddr);
		for (p = dbuf; p < dbufp; p++) {
		    switch ( p->lv ) {
		    case LV_BYTE: fmt = "%02x "; lstaddr += 1; break;
		    case LV_WORD: fmt = "%04x "; lstaddr += 2; break;
		    case LV_LONG: fmt = "%08x "; lstaddr += 4; break;
		    default: ;
		    }
		    fprintf (lstfile, fmt, p->val);
		}
		fprintf (lstfile, "\n");
	    }
	    dbufp = dbuf;
	    nlflg = 0;
	}

	if (unget_ch != -1) {
	    ch = unget_ch;
	    unget_ch = -1;

	} else {
	    if ((ch = get_ch()) == '-') {
		if ((ch = get_ch()) == '-')
		    while ((ch = get_ch()) != '\n' && ch != EOF);
		else {unget_ch = ch; ch = '-';}
	    }
	}
	if (ch == '\n') {
	    lineno++;
	    ch = ' ';
	    nlflg++;
	    kanji2 == 0;
	} else if (ch != EOF) {
#ifdef	WIN
	    if (kanji2 == 1) {
		kanji2 = 0;
	    } else {
		if (KANJI(ch)) {
		    kanji2 = 1;
		}
		if (flg) {
		    if (ch < ' ' && ch != '\t')
			ch = ' ';
		} else {
		    if (ch <= ' ' || ch == ',' || ch == ';' || ch =='(' ||
			ch == ')' || ch == '{' || ch == '}') ch = ' ';
		}
	    }
#endif
	    if (flg) {
		if (ch < ' ' && ch != '\t') ch = ' ';
	    } else {
		if (ch < ' '  || ch == ',' || ch == ';' || ch == '(' ||
		    ch == ')' || ch == '{' || ch == '}' )  ch = ' ';
	    }
	}
	return (ch);
}
/*----------------------------------------------------------------------*/
skip_space()					/* skip space		*/
/*----------------------------------------------------------------------*/
{
	while (ISSP(ch)) in_ch(0);
}
/*----------------------------------------------------------------------*/
next_ch()				/* get next non-space char	*/
/*----------------------------------------------------------------------*/
{
	do {in_ch(0);} while (ISSP(ch));
}
/*----------------------------------------------------------------------*/
WORD search_symbol (sym)			/* search symbol	*/
/*----------------------------------------------------------------------*/
BYTE	*sym;
{
	FAST WORD	i;

	for (i = max_sym; --i >= 0; ) {
	    if ( (symbol[i].level == sym_level || symbol[i].level == 0) &&
			strncmp (sym, symbol[i].nam, 16) == 0) break;
	}
	return (i);
}
/*----------------------------------------------------------------------*/
LONG get_symbol (p, pp, lv)			/* get symbol value	*/
/*----------------------------------------------------------------------*/
FAST BYTE	*p;
BYTE		**pp;
LV		*lv;	/* ����ܥ��ͤΥӥå�Ĺ���֤� */
{
	FAST WORD	i, f;
	BYTE		sym[17];

	memset (sym, 0, 17);
	for (i = 0; isalnum(*p) || *p == '_'; p++) {if (i < 16) sym[i++] = *p;}

	f = 0;
	*lv = symbol[0].lv; /* BASE */
	if (*p == ':') {
	    f = 1; p++;
	    switch (*p++) {
	    case 'L':
		*lv = LV_LONG;	break;
	    case 'W':
	    case 'H':
		*lv = LV_WORD;	break;
	    case 'Y':
		*lv = LV_BYTE;	break;
	    default:
		f = 0; p--;
	    }
	}
	*pp = p;

	/* local symbols */
	if ((i = search_symbol(sym)) >= 0) {
	    if (f == 0) {
		if ((*lv = symbol[i].lv) == LV_UNDEF)
		    *lv = symbol[0].lv; /* BASE */
	    }
	    return ((i == 1)? (ftell(tmpf) - id_base) : symbol[i].val);
	}

	if (pass2) errexit ("Undefined Symbol", sym);
	return (0L);
}
/*----------------------------------------------------------------------*/
LONG get_value (vp)				/* get value		*/
/*----------------------------------------------------------------------*/
LONG	*vp;
{
	FAST WORD	i;
	FAST LONG	v;
	LV		lv, l;
	BYTE		*p;

	*vp = 0;
	if (!isalnum(ch) && ch != '-') return (-1);

	for (i = 0; !ISSP(ch);in_ch(0))
		if (i < BUF_SIZE)
			buf[i++] = ch;
		else
			fprintf (stderr,"WARNING: value too long AT LINE %d\n", lineno);

	buf[i] = 0;

	lv = LV_BYTE;
	i = 0; /* + */
	for (p = buf; *p; ) {
	    bufp = p;
	    if (isalpha(*p)) {
		v = get_symbol (p, &p, &l);
	    } else {
#ifdef	WIN
		v = strtoul ((char *) p, (char *) &p, (int) symbol[0].val);
#else
		v = strtoul (p, &p, (WORD)symbol[0].val);
#endif
		switch ( *p++ ) {
		case 'L': case 'l':	l = LV_LONG; break;
		case 'H': case 'h':	l = LV_WORD; break;
		case 'Y': case 'y':	l = LV_BYTE; break;
		default:		l = symbol[0].lv; --p; /* BASE */
		}
	    }
	    if ( l > lv ) lv = l;

	    if (i == 1) 	*vp -= v;
	    else if (i == 2)	*vp *= v;
	    else if (i == 3)	*vp /= v;
	    else		*vp += v;

	    if (*p == '+') {i = 0; p++;}
	    else if (*p == '-') {i = 1; p++;}
	    else if (*p == '*') {i = 2; p++;}
	    else if (*p == '/') {i = 3; p++;}
	    else if (*p) errexit ("Illegal Value", p);
	}
	return (lv);
}
/*----------------------------------------------------------------------*/
def_symbol (sym, v, lv, warn)			/* define symbol	*/
/*----------------------------------------------------------------------*/
BYTE	*sym;
LONG	v;
LV	lv;
WORD	warn;
{
	FAST WORD	i;

	if ((i = search_symbol(sym)) < 0) {
	    i = max_sym++;
	    if (max_sym >= MAX_SYM) errexit ("too many symbols", sym);
	    symbol[i].level = (warn & 0x1000)? 0 : sym_level;
	} else if (pass2 == 0 && i && i != 2) {
		if (i < system_symbols)
fprintf (stderr, "WARNING: system symbol '%s' is re-defined AT LINE %d\n",
					sym, lineno);
		else if (warn & 1)
fprintf (stderr, "WARNING: symbol '%s' is multi-defined AT LINE %d\n",
					sym, lineno);
	}
	strncpy (symbol[i].nam, sym, 16);
	symbol[i].val = v;
	symbol[i].lv = lv;

}
/*----------------------------------------------------------------------*/
init_symbol ()					/* initialize symbols	*/
/*----------------------------------------------------------------------*/
{
	system_symbols = 0;

	system_symbols++; def_symbol ("BASE",	0L,  LV_WORD,  1);
							/* must be 1st Entry */
	system_symbols++; def_symbol ("OFFSET", 0L,  LV_UNDEF, 1);
							/* must be 2nd Entry */
	system_symbols++; def_symbol ("ALIGN", 1L,  LV_UNDEF, 1);
							/* must be 3rd Entry */
	system_symbols++; def_symbol ("PTR_DATA",    1L,  LV_WORD, 1);
	system_symbols++; def_symbol ("PICT_DATA",   2L,  LV_WORD, 1);
	system_symbols++; def_symbol ("PAT_DATA",    3L,  LV_WORD, 1);
	system_symbols++; def_symbol ("BMAP_DATA",   4L,  LV_WORD, 1);
	system_symbols++; def_symbol ("FIG_DATA",    5L,  LV_WORD, 1);
	system_symbols++; def_symbol ("TEXT_DATA",   6L,  LV_WORD, 1);
	system_symbols++; def_symbol ("PARTS_DATA",  7L,  LV_WORD, 1);
	system_symbols++; def_symbol ("MENU_DATA",   8L,  LV_WORD, 1);
	system_symbols++; def_symbol ("GMENU_DATA",  9L,  LV_WORD, 1);
	system_symbols++; def_symbol ("PANEL_DATA", 10L,  LV_WORD, 1);
	system_symbols++; def_symbol ("USER_DATA",  64L,  LV_WORD, 1);
}
/*----------------------------------------------------------------------*/
put_symbol()					/* put symbol value	*/
/*----------------------------------------------------------------------*/
{
	FAST WORD	i;
	FAST WORD	warn = 0;
	LONG		v;
	LV		lv;
	BYTE		sym[17];

	next_ch();
	if (ch == '.') next_ch();
	else warn++;

	if (ch == '!') { next_ch(); warn |= 0x1000;}

	if (!isalpha(ch)) errexit ("Illegal Symbol format", NULL);

	memset (sym, 0, 17);
	for (i = 0; isalnum(ch) || ch == '_'; in_ch(0))
					if (i < 16) sym[i++] = ch;
	skip_space();
	if (ch == ':') {
	   {				/* .ALIGN */
		W	align;

		align = symbol[2].val - 1;	/* bit mask*/
		if (align < 0)
			align = 0;
		while (dcnt & align)
			put_byte(0);
	   }
	   v = ftell(tmpf) - id_base;
	   lv = LV_UNDEF;
	   next_ch();
	} else {
	   if (ch != '=') errexit ("Illegal Symbol Definition format", NULL);
	   next_ch();
	   lv = get_value (&v);
	}
	def_symbol (sym, v, lv, warn);
}
/*----------------------------------------------------------------------*/
put_byte(n)				/* put byte to temp file	*/
/*----------------------------------------------------------------------*/
BYTE	n;
{
	if (pass2) {
		dbufp->val = (LONG)n & 0xffL;
		dbufp->lv = LV_BYTE;
		dbufp++;
	}
	fputc(n, tmpf);
	dcnt += 1;
}
/*----------------------------------------------------------------------*/
put_word(n)				/* put word to temp file	*/
/*----------------------------------------------------------------------*/
WORD	n;
{
	if (pass2) {
		dbufp->val = (LONG)n & 0xffff;
		dbufp->lv = LV_WORD;
		dbufp++;
	}
	n = CWORD(n);
	fputc(U_BYTE(n), tmpf);
	fputc(L_BYTE(n), tmpf);
	dcnt += 2;
}
/*----------------------------------------------------------------------*/
put_long(n)				/* put long to temp file	*/
/*----------------------------------------------------------------------*/
LONG	n;
{
	if (pass2) {
		dbufp->val = n;
		dbufp->lv = LV_LONG;
		dbufp++;
	}
	n = CLONG(n);
	fputc((n >> 24) & 0xff, tmpf);
	fputc((n >> 16) & 0xff, tmpf);
	fputc((n >>  8) & 0xff, tmpf);
	fputc((n      ) & 0xff, tmpf);
	dcnt += 4;
}
/*----------------------------------------------------------------------*/
WORD hex(c)					/* get hex char value	*/
/*----------------------------------------------------------------------*/
WORD	c;
{
	if (isdigit(c)) return (c -'0');
	if (c >= 'A' && c <='F') return ( c -'A' + 10);
	if (c >= 'a' && c <='f') return ( c -'a' + 10);
	return (-1);
}
/*----------------------------------------------------------------------*/
put_kstring()				/* put kanji string "..."	*/
/*----------------------------------------------------------------------*/
{
	FAST WORD	i, n, j, cnt;
	UWORD	k, jisch;

	in_ch(1);
	cnt = 0;
	while ( ch != '"' ) {
	    if (KANJI(ch)) {
	    	if (HANKANA(ch)) {
		    fprintf(stderr,
			"WARNING: hankaku KANA exists AT LINE %d\n",lineno);
		    n = ch;
#ifndef	WIN
		    in_ch(1);		/* Skip KANA Flag (not WIN)*/
#endif
		    jisch = n << 8 | ch;
		} else {
		    n = ch;
		    in_ch(1);
		    k = n << 8 | ch;
		    jisch = chgtojis(k);
		}
		put_word(jisch);
		cnt++;
	    } else {
		if (ch == EOF) return;
		if (ch == '\\') {
		    if (in_ch(1) >= '0' && ch <= '7') {
			n = 0;
			for (i = 0; i < 3; i++) {
			    n = n * 8 + ch - '0';
			    if (in_ch(1) < '0' || ch > '7') break;
			}
			put_word (PAD_WORD(n));
			cnt++;
			continue;
		    }
		    if (ch == 'X' || ch == 'x') {
			n = 0;
			for (i = 0; i < 4; i++) {
			    in_ch(1);
			    if ((j = hex(ch)) < 0) break;
			    n = n * 16 + j;
			}
			put_word (n);
			cnt++;
			if (i >= 4) in_ch(1);
			continue;
		    }
		    if (ch == 'n') ch = '\n';
		    else if (ch == 'f') ch = '\f';
		    else if (ch == 't') ch = '\t';
		    else if (ch == 'r') ch = '\r';
		    else if (ch == 'b') ch = '\b';
		} else {
		    fprintf (stderr,
		"WARNING: hankaku '%c' exists AT LINE %d\n", ch, lineno);
		}
		put_word (PAD_WORD(ch));
		cnt++;
	    }
	    in_ch(1);
	}

	in_ch(0);
	if (isdigit(ch)) {
	    n = 0;
	    do {n = n * 10 + (ch - '0'); in_ch(0);} while (isdigit(ch));
	    if ((n -= cnt) > 0) do {put_word(0);} while (--n);
	}
	skip_space();
}

/*----------------------------------------------------------------------*/
put_string()				/* put ASCII string '...'	*/
/*----------------------------------------------------------------------*/
{
	FAST WORD	i, c, j, cnt, kcnt, pc;

	in_ch(1);
	cnt = kcnt = 0;
	while ( (c = ch) != '\'' ) {
	    if (KANJI(c)) {
#ifdef 	WIN
		if (HANKANA(c))		/* hankaku kana */
#else
		if ((++kcnt & 1) && c == 0250)	/* hankaku kana */
#endif
		    fprintf(stderr,
			"WARNING: hankaku KANA exists AT LINE %d\n",lineno);
	    } else {
		if (c == EOF) return;
		if (c == '\\') {
		    if (in_ch(1) >= '0' && ch <= '7') {
			for (c = i = 0; i < 3; i++) {
			    c = c * 8 + ch - '0';
			    if (in_ch(1) < '0' || ch > '7') break;
			}
			goto NEXT;
		    } else if (ch == 'X' || ch == 'x') {
			for (c = i = 0; i < 2; i++) {
			    in_ch(1);
			    if ((j = hex(ch)) < 0) break;
			    c = c * 16 + j;
			}
			if (i < 2) goto NEXT;
		    } else {
			if (ch == 'n')	    c = '\n';
			else if (ch == 'f') c = '\f';
			else if (ch == 't') c = '\t';
			else if (ch == 'r') c = '\r';
			else if (ch == 'b') c = '\b';
			else		    c = ch;
		    }
		}
	    }
	    in_ch(1);
NEXT:
	    if ((++cnt) & 1)	pc = c;
	    else		put_word (TO_XWORD(pc, c));
	}

	if (cnt & 1)  {put_word (TO_XWORD(pc, 0)); cnt++;}

	in_ch(0);
	if (isdigit(ch)) {
	    i = 0;
	    do {i = i * 10 + (ch - '0'); in_ch(0);} while (isdigit(ch));
	    if ((i -= cnt / 2) > 0) do {put_word(0);} while (--i);
	}
	skip_space();
}

/*----------------------------------------------------------------------*/
put_img()				/* put image data `...`		*/
/*----------------------------------------------------------------------*/
{
	FAST WORD	i, c, j, cnt, kcnt, pc;
	WORD		n;
	WORD		bcnt;
	WORD		byte;
#define	C_ON		0x2223
#define	C_OFF		0x2222
#define	A_OFF		0x2e

	in_ch(1);
	byte = bcnt = cnt = 0;
	while ( (c = ch) != '`' ) {
	    if (KANJI(c)) {
		in_ch(1);
		n = ((c << 8) & 0xff00) | (ch &0xff);
		c = chgtojis(n);

		if (c == C_ON) {
		    byte = (byte << 1) | 1;
		} else if (c == C_OFF) {
		    byte = byte << 1;
		} else {
skip_char:
		    in_ch(1);
		    continue;
		}
	    } else {
		if (isalnum(c)) {
		    byte = (byte << 1) | 1;
		} else if (c == A_OFF) {
		    byte = byte << 1;
		} else {
		    goto skip_char;
		}
	    }

	    in_ch(1);
	    if (((++bcnt) % 8) == 0) {
		if ((++cnt) & 1)	pc = byte;
		else			put_word (TO_XWORD(pc, byte));
		bcnt = byte = 0;
	    }
	}

	if ((bcnt % 8) != 0) {
	    byte <<= (8 - bcnt);
	    if ((++cnt) & 1)	pc = byte;
	    else		put_word (TO_XWORD(pc, byte));
	}
	if (cnt & 1)  {put_word (TO_XWORD(pc, 0)); cnt++;}

	in_ch(0);
#if 0
	if (isdigit(ch)) {
	    i = 0;
	    do {i = i * 10 + (ch - '0'); in_ch(0);} while (isdigit(ch));
	    if ((i -= cnt / 2) > 0) do {put_word(0);} while (--i);
	}
#endif
	skip_space();
}

/*----------------------------------------------------------------------*/
put_bmap()				/* put bmap data		*/
/*----------------------------------------------------------------------*/
{
	WORD		i, c, j, cnt, pc;
	WORD		n;
	LONG		v;
	LONG		size;
	LONG		psz;
	LONG		compac, planes, pixbits, rowbytes;
	WORD		left, top, right, bottom;
#define	CBMP_SIZE	(4 + 4 + 2 + 2 + 8)

	WORD	pixelbits;	/* 1, 2, 4, 8 */
	WORD	pixelmask;	/* 1, 3, 15, 255 */
	WORD	shiftcount;	/* ���ӥå��ɤ߹�������������(<8) */
	WORD	widthcount;	/* ���ӥå��ɤ߹�������������(<rowbytes*8) */
	WORD	buffer;
	WORD	imgonly;

	/* $CBMP(compac planes pixbits rowbyte rect data) */
	/* $CBMPIMG(compac planes pixbits rowbyte rect data) */
	do { in_ch(1); } while(isspace(ch));
	if (ch != 'C') goto EEXIT;
	in_ch(1);
	if (ch != 'B') goto EEXIT;
	in_ch(1);
	if (ch != 'M') goto EEXIT;
	in_ch(1);
	if (ch != 'P') {
EEXIT:		errexit ("Must $CBMP or $CBMP_IMG", NULL);
	}
	in_ch(1);
	imgonly = (ch == 'I') ? 1 : 0;

	for(; ch != '('; ) in_ch(1);

	do { in_ch(0); } while(isspace(ch));
	get_value(&compac);

	do { in_ch(0); } while(isspace(ch));
	get_value(&planes);

	do { in_ch(0); } while(isspace(ch));
	get_value(&v);
	pixbits = v;

	do { in_ch(0); } while(isspace(ch));
	get_value(&v);
	rowbytes = v;

	do { in_ch(0); } while(isspace(ch));
	get_value(&v);
	left = v;

	do { in_ch(0); } while(isspace(ch));
	get_value(&v);
	top = v;

	do { in_ch(0); } while(isspace(ch));
	get_value(&v);
	right = v;

	do { in_ch(0); } while(isspace(ch));
	get_value(&v);
	bottom = v;

	if (planes != 1 || compac != 0 ||(rowbytes & 1))
	    errexit ("Illegal CBMP format", NULL);

	switch (pixbits) {
		case	0x0101:
			pixelbits = 1;
			pixelmask = 1;
			shiftcount = widthcount = 0;
			break;
		case	0x0202:
			pixelbits = 2;
			pixelmask = 3;
			shiftcount = widthcount = 0;
			break;
		case	0x0404:
			pixelbits = 4;
			pixelmask = 15;
			shiftcount = widthcount = 0;
			break;
		case	0x0808:
			pixelbits = 8;
			pixelmask = 255;
			shiftcount = widthcount = 0;
			break;
		default:
			errexit ("Illegal CBMP format", NULL);
	}

	size = CBMP_SIZE + (psz = rowbytes * (bottom - top)) + 4 * planes;

	if (imgonly == 0) {

		put_long(compac);
		put_long(planes);
		put_word(pixbits);
		put_word(rowbytes);

		put_word(left);		put_word(top);
		put_word(right);	put_word(bottom);

		v = CBMP_SIZE + 4;
		put_long(v);
	}

	in_ch(1);
	cnt = 0;
	while ( (c = ch) != ')' ) {
		if (widthcount >=(right - left))
			c = 0;
		else {
		    if (KANJI(c)) {
			in_ch(1);
			n = ((c << 8) & 0xff00) | (ch & 0xff);
			c = chgtojis(n);

			if (c == C_ON) {
			    c = 0xff;
			} else if (c == C_OFF) {
			    c = 0;
			} else {
skip_char:
			    in_ch(1);
			    continue;
			}
		    } else {
			if (isalpha(c)) {
			    c = 0xff;
			} else if (isxdigit(c)) {
			    if ('0' <= c && c <= '9') c = c - '0';
			    else if ('a' <= c && c <= 'f') c = c - 'a' + 10;
			    else c = c - 'A' + 10;
			} else if (c == A_OFF) {
			    c = 0;
			} else {
			    goto skip_char;
			}
		    }
		    in_ch(1);
		}
		if (shiftcount == 0) {
			buffer = 0;
			shiftcount = 8;
		}
		shiftcount -= pixelbits;
		buffer |= ((c & pixelmask) << shiftcount);
		if ((++widthcount) >= rowbytes * 8 / pixelbits)
			widthcount = 0;
		if (shiftcount)
			continue;
		c = buffer;

		if ((++cnt) & 1)
			pc = c;
		else
			put_word (TO_XWORD(pc, c));
	}
	
	if (shiftcount == 0)
		;
	else if ((++cnt) & 1)
		pc = buffer;
	else
		put_word (TO_XWORD(pc, buffer));
	
	if (cnt & 1) {
		put_word (TO_XWORD(pc, 0));
		cnt++;
	}
	/* �ǡ������ץ졼��Υ�������꾮������� 0 ������ */
	for (; cnt < psz; cnt += 2) put_word(0);

	in_ch(0);
	skip_space();
}

/*----------------------------------------------------------------------*/
make_index()					/* make resource index	*/
/*----------------------------------------------------------------------*/
{
	LONG	ty, attr;

	next_ch();
	get_value (&ty);
	if (ty <= 0 || ty > 0x7fff) errexit ("Illegal DATA Type", bufp);

	skip_space();
	get_value(&attr);			/* optional att */

	if (tycnt) {				/* set index */
	    dp->npos = (BYTE *) &(dp->dix[idno]) - dps;
	    dp = (DTYP_INX *) &(dp->dix[idno]);
	}

	dp->npos = 0L;
	dp->typ = XCLONG(ty);
	dp->attr = XCLONG(attr);
	dp->nd = 0;
	idno = 0;
	tycnt++;
}

/*----------------------------------------------------------------------*/
make_id()					/* make ID record	*/
/*----------------------------------------------------------------------*/
{
	LONG	id, opt, x;

	next_ch();			/* get id number */
	get_value (&id);
	if (id < 0 || id > 0x7fff) errexit ("Illegal DATA ID", bufp);

	dp->dix[idno].id = XCLONG(id);
	dp->dix[idno].pos = id_base = ftell(tmpf);
	dp->dix[idno].size = 0;

	skip_space();
	get_value (&opt);		/* get optional info */
	dp->dix[idno].info[0] = XCLONG(opt);
	skip_space();
	get_value (&opt);		/* get optional info */
	dp->dix[idno].info[1] = XCLONG(opt);
	dp->nd = ++idno;
	dcnt = 0;
	sym_level++;
}
/*----------------------------------------------------------------------*/
fin_make_id()					/* make ID record	*/
/*----------------------------------------------------------------------*/
{
	dp->dix[idno - 1].size = XCLONG(dcnt);
}
/*----------------------------------------------------------------------*/
make_data()					/* make data file main	*/
/*----------------------------------------------------------------------*/
{
	FAST WORD	i, n, seq;
	LONG		v, l;
	DBOX_HDR	*hp;

	/* seq = 0:	to be INDEX (initial)
		 1:	to be DNUM
		 2:	to be DATA
		 3:	to be DATA-END (INDEX or DNUM)	*/


	dps = (BYTE *)malloc (INDEXSZ);		/* memory for data indexes */
	max_sym = 0;
	sym_level = 0;
	init_symbol();
	pass2 = 0;
	lstaddr = 0;

AGAIN:
#if 1
	symbol[0].lv  = LV_WORD;	/* .BASE �����ͤˤ���*/
	symbol[2].val  = 1;	/* .ALIGN �����ͤˤ���*/
#endif
	dp = (DTYP_INX *)(dps + sizeof(DBOX_HDR));

	tycnt = 0;
	sym_level = 0;
	lineno = 1;
#if 0
	seq = (noheader)? 2 : 0;
#else
	if (noheader) {
		seq = 2;
		dcnt = 0;		/*�ɲ�*/
	} else
		seq = 0;
#endif
	dbufp = dbuf;

	for (next_ch(); ch != EOF; ) {

	    if (isalnum(ch) || ch == '"' || ch == '\'' || ch == '-' ||
						ch == '`' || ch == '$') {
		if (seq < 2) errexit ("Missing INDEX or DNUM", NULL);
		seq = 3;
		if (ch == '"')		put_kstring ();
		else if (ch == '\'')	put_string ();
		else if (ch == '`')	put_img ();
		else if (ch == '$')	put_bmap ();
		else {
			switch (get_value(&v)) {
			case LV_BYTE: put_byte (v); break;
			case LV_WORD: put_word (v); break;
			case LV_LONG: put_long (v); break;
			default: errexit ("Missing Value", NULL);
			}
		}

	    } else if (ch == '.') {
		put_symbol();

	    } else if (ch == '%') {
		if (seq == 1 || seq == 2 || noheader)
				errexit ("Missing DNUM or DATA", NULL); 
		if (seq == 3) fin_make_id ();
		seq = 1;
		make_index();

	    } else if (ch == '#') {
		{				/* .ALIGN */
			W	align;

			align = symbol[2].val - 1;	/* bit mask*/
			if (align < 0)
				align = 0;
			while (dcnt & align)
				put_byte(0);
		}
		if (seq == 0 || seq == 2 || noheader)
				errexit ("Missing INDEX or DATA", NULL);
		if (seq == 3) fin_make_id ();
		seq = 2;
		make_id();

	    } else errexit ("Illegal data format", NULL);

	    skip_space();
	}

	if (seq != 3) errexit ("Missing DATA", NULL);
	fin_make_id ();

	/* make header */
	if (!noheader) {
	    hp = (DBOX_HDR *)dps;
	    hp->ntyp = XCWORD(tycnt);
	    n = (BYTE *) &(dp->dix[idno]) - dps;
	    hp->ixsize = XCWORD(n);
	    for (i = 0; i < 6; i++) hp->resv[i] = 0;

	    dp = (DTYP_INX *) (dps + sizeof(DBOX_HDR));
	    do {
		l = dp->npos;
		dp->npos = XCLONG(l);
		i = dp->nd;
		dp->nd = XCLONG(i);
		while (--i >= 0) {
		    v = dp->dix[i].pos + n;
		    dp->dix[i].pos = XCLONG(v);
		}
		dp = (DTYP_INX *)(l + dps);
	    } while (l);
	}

	if (pass2++ == 0) {
	    rewind(tmpf);
	    rewind(infile);
	    if (lstfile) {
		lstaddr = /* n */ 0;
		if (!noheader) dump_header(n);
	    }
	    goto AGAIN;
	}

	/* output header */
	if (!noheader && !source) {
	    if (fwrite (dps, 1, n, oufile) != n) eexit ("File write error");
	}

	/* output data */
	rewind (tmpf);
	if ( source > 0 ) {
	    fprintf(oufile, "/* DATABOX Data */\n");
	    fprintf(oufile, "unsigned %s\tdatabox_data[] = {\n",
				( source == 2 )? "short": "long");
	    seq = 0;
	    while ( (i = fgetc(tmpf)) != EOF ) {
		if ( (seq % 16) > 0 )	fprintf(oufile, ", ");
		else if ( seq == 0 )	fprintf(oufile, "\t");
		else { seq = 0;		fprintf(oufile, ",\n\t"); }
		v = i;
		while ( (++seq % source) > 0 ) {
			v <<= 8;
			if ( i != EOF && (i = fgetc(tmpf)) != EOF ) v |= i;
		}
		if ( source == 2 )	fprintf(oufile, "0x%04x", CWORD(v));
		else			fprintf(oufile, "0x%08x", CLONG(v));
	    }
	    fprintf(oufile, "\n};\n");
	} else {
		while  ((i = fgetc(tmpf)) != EOF) fputc(i, oufile);
	}
}
/*----------------------------------------------------------------------*/
WORD	*xdump(n)
/*----------------------------------------------------------------------*/
WORD	n;
{
	WORD	*p;
static	LONG	adr = 0;
	WORD	c;

	fprintf (lstfile, "=%04x: ", adr);
	for (p = (WORD *)&dps[adr]; n > 0; n -= 2, adr += 2) {
		c = *p++;
		c = XCWORD(c);
		fprintf (lstfile, "%04x ", ((LONG)c) & 0xffff);
	}
	fprintf (lstfile, "\n");
	return (p);
}
/*----------------------------------------------------------------------*/
dump_header(sz)
/*----------------------------------------------------------------------*/
WORD	sz;
{
	WORD	*p;
	LONG	n;

	fprintf (lstfile, "**** HEADER PART ***\n");
	p = xdump (sizeof(DBOX_HDR));
	sz -= sizeof(DBOX_HDR);

	while (sz > 0) {
	     n = ((DTYP_INX *)p)->nd;
	     n = XCLONG(n);
	     xdump (sizeof(DTYP_INX) - sizeof(DNUM_INX));
	     sz -= sizeof(DTYP_INX) - sizeof(DNUM_INX);
	     while (n--) {
		p = xdump (sizeof(DNUM_INX));
		if ((sz -= sizeof(DNUM_INX)) <= 0) break;
	     }
	}
	fprintf (lstfile, "**** DATA PART ***\n");
}
/*----------------------------------------------------------------------*/
usage()
/*----------------------------------------------------------------------*/
{
printf ("Usage: databox [-b][-n][-l][-s[a]] [input_file] [output_file]\n");
printf ("       -b: Big Endian, -n: No header, -l: listing\n");
printf ("       -s: Source Out, a: alignment type is long\n");
exit(0);
}
/*----------------------------------------------------------------------*/
xexit ()
/*----------------------------------------------------------------------*/
{
#ifdef	WIN
	if (oufile && ofn[0]) unlink ((char *)ofn);
	if (lstfile)  unlink ((char *)ifn);
#else
	if (oufile && ofn[0]) unlink (ofn);
	if (lstfile)  unlink (ifn);
#endif
	exit(1);
}
/*----------------------------------------------------------------------*/
eexit (mess)
/*----------------------------------------------------------------------*/
BYTE	*mess;
{
	fprintf (stderr,"ERROR: %s\n",mess);
	xexit();
}
/*----------------------------------------------------------------------*/
errexit (mess, err)
/*----------------------------------------------------------------------*/
BYTE	*mess;
BYTE	*err;
{
	BYTE	c[4];

	if (err == NULL) {
	    c[0] = ch; c[1] = 0; c[2] = 0;
	    if (ch == EOF) c[0] = '$';
	    else if (KANJI(ch)) {in_ch(0); c[1] = ch;}
	    err = c;
	}

	fprintf (stderr,"ERROR: %s AT '%s' LINE %d\n",mess, err, lineno);
	xexit();
}
#ifdef	WIN
/*----------------------------------------------------------------------*/
UH	sjistojis (UH sj)           /* SJIS -> JIS (MS-DOS or Win Only) */
/*----------------------------------------------------------------------*/
{
	UB	sc1, sc2;

	sc1 = ((sj >> 8) & 0xff);
	sc2 = ( sj       & 0xff);
	sc1 -= (sc1 < 0xa0) ? 0x71: 0xb1;
	sc1 = sc1 + sc1 + 1;
	if (sc2 > 0x7f) sc2--;
	if (sc2 >= 0x9e) {
		sc1++;
		sc2 -= 0x7d;
	} else {
		sc2 -= 0x1f;
	}
	return (sc1 << 8) | sc2;
}
#endif
