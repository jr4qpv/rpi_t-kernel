/*
	load.c		gterm : ���ɥ��ޥ�ɽ���

	Copyright (C) 1996-2012 by Personal Media Corporation
*/
#include	<stdio.h>
#include	<setjmp.h>
#include	<signal.h>
#include	<ctype.h>
#include	<string.h>

#include	"devio.h"
#include	"gterm.h"
#include	"crctable.h"		/* CRC �ơ��֥�		*/

#ifdef	USE_LOADCMD

#define		EVAL_TIME		/* ���֤�¬��		*/

static	FILE		*loadfp = NULL;
static	sigjmp_buf	load_jmp;

/* ���楳���� */
#define SOH	(0x01)
#define STX	(0x02)
#define EOT	(0x04)
#define ACK	(0x06)
#define NAK	(0x15)
#define CTLZ	('Z'-'@')
#define	CAN	('X'-'@')

#define	DEF_BLKSZ	128		/* �֥�å�������	*/
#define	XBLKSZ		1024		/* �֥�å������� (1K)	*/
#define	MAX_RETRY	20		/* �����ȥ饤��	*/

/*
	��������� 1 ʸ��������
	��  ��: int tmout	>= 0	�����ॢ����(��)
				-1	�����ॢ���Ȥʤ�
	�����:			>= 0	��������� 1 ʸ��
				-1	�����ॢ����
			
*/
static	int	RSgetx(int tmout)
{
	int	c;

	if (tmout > 0) tmout *= 1000; /* �ߥ��ä��Ѵ� */
	c = RSgetc(1, tmout);
	return c;
}

/*
	XMODEM �Υ��ܡ��Ȥ�����
*/
static	void	send_abort(void)
{
	RSoutx(CAN);
	RSoutx(CAN);
	RSoutx(CAN);
}

/*
	XMODEM �ˤ������
*/
static	int	xmodem_send(int bnum, char *bufp, int dlen)
{
static	int	USE_CRC = 0;
	int	i, i2, retry, ck, blksz = 0;
	char	*bp;

	if (bnum <= 0) {	/* �ǽ� */
		for (;;) {
			if ((i = RSgetx(60)) == NAK) {
				USE_CRC = 0;
				break;
			}
			if (i == 'C') {
				USE_CRC = 1;
				break;
			}
			if (i == CAN) goto XCANCEL;
			if (i == -1) {
				send_abort();
				goto XCANCEL;
			}
		}
	}

	if (dlen > 0) {
		blksz = (dlen <= DEF_BLKSZ) ? DEF_BLKSZ : XBLKSZ;
		bp = bufp + 3;
		while (dlen < blksz) bp[dlen++] = CTLZ;
		if (USE_CRC) {
			for (ck = i = 0; i < blksz; i++) {
				ck = (ck << 8) ^ crc_table[((ck >> 8) ^
						(uhalf)(*bp++)) & 0xff];
			}
			*bp++ = ck >> 8;
			*bp = ck & 0xff;
		} else {
			for (ck = i = 0; i < blksz; i++) ck += *bp++;
			*bp = ck & 0xff;
		}
	}

	bnum = (bnum + 1) & 0xff;
	for (retry = 0; retry < MAX_RETRY; retry++) {
		if (dlen <= 0) {
			RSoutx(EOT);
		} else {
			bufp[0] = (blksz == DEF_BLKSZ) ? SOH : STX;
			bufp[1] = bnum;
			bufp[2] = 0xff - bnum;
			RSwrite(bufp, blksz + 3 + (USE_CRC ? 2 : 1));
		}
		for (;;) {
			if ((i = RSgetx(60)) == ACK) return (dlen <= 0) ? 1 : 0;
			if (i == NAK) break;
			if (i == CAN && (i2 = RSgetx(60)) == CAN) goto XCANCEL;
			if (i == -1 || i2 == -1) {
				send_abort();
				goto XCANCEL;
			}
		}
		Putc('!');
		TYflush();
	}
	send_abort();
	Print(".. Retry over");
	return -1;

XCANCEL:
	Print(".. Cancelled");
	return -1;
}

/*
	���ɤ�����
*/
static	void	load_break(void)
{
	siglongjmp(load_jmp, 1);
}

/*
	���ɽ���
*/
int	do_load(int kind, uchar *p)
{
	int		i, n, k;
	uchar		buf[1024+64], cmd[32], head[16];
	char		form;
	int		sz, tsz;
#ifdef	EVAL_TIME
	long		tm;
#endif

	/* �ե�����Υ����ץ� */
	get_term(buf, &p);
	if (!(loadfp = f_open (buf, "r"))) {
		if (kind & kRECV) RSputc(NAK);
		Print(".. Can't open: %s", buf);
		goto EXIT;
	}

	form = 'M';			/* ���ꥤ�᡼�� */
	if (!(kind & kRECV)) {
		if (!(kind & kMEMIMG)) {
			/* �ե���������Υ����å� */
			if ((sz = fread(head, 1, sizeof(head), loadfp)) <= 0) {
				Print(".. Can't read");
				goto EXIT;
			}
			if (sz >= sizeof(head)) {
				if (head[0] == 'S') {
					form = 'S';	/* S �ե����ޥå� */
				}
			}
		}

		/* �������륳�ޥ�ɤ����� */
		if (kind & kFLLO) {		/* Flash ROM Load */
			if (form != 'S') {
				Print(".. Not S-Format file");
				goto EXIT;
			}
			if (*p == '@') {	/* Custom Command */
				for (i = 0, p++; i < 30 && *p; i++)
					cmd[i] = *p++;
			} else {		/* FLLO [E][X] command */
				strcpy(cmd, "FLLO ");
				for (i = 5; *p; p++) {
					switch(*p) {
					case 'X':
					case 'x': kind |= kXMODEM;
						  break;
					case 'E':
					case 'e': if (i == 5) cmd[i++] = 'E';
						  break;
					default: Print(".. Bad option %c", *p);
						 goto EXIT;
					}
				}
			}
		} else {			/* LO [X][S] command */
			if (!(kind & kXMODEM) && form == 'M') {
				Print(".. Use xmodem");
				goto EXIT;
			}
			strcpy(cmd, "LO ");
			i = 3;
		}
		if (kind & kXMODEM) cmd[i++] = 'X';
		if (!(kind & kFLLO)) {
			cmd[i++] = form;
		}
		cmd[i] = '\0';

		if (*p) {	/* ���ɥ쥹���� */
			cmd[i++] = ',';
			get_term(&cmd[i], &p);
		} else if (form == 'M') {
			Print(".. No load address");
			goto EXIT;
		}
	}

	/* �ե����륵�����μ��� */
	fseek(loadfp, 0, 2);
	tsz = ftell(loadfp);
	rewind(loadfp);

	/* ���ǥ���ȥ������ */
	if (sigsetjmp(load_jmp, 0)) {
		sigset_t set;

		Puts("^C");
		if (kind & kXMODEM) {
			while ((n = RSgetx(10)) != NAK && n != ACK && n != -1);
		}
		send_abort();

		sigemptyset(&set);
		sigaddset(&set, SIGINT);
		sigprocmask(SIG_UNBLOCK, &set, NULL);
		goto EXIT;
	}
	signal(SIGINT, (void*)load_break);

#ifdef	EVAL_TIME
	tm = time(NULL);
#endif

	/* ���ޥ�ɤ����� */
	if (kind & kRECV) {
		sprintf(cmd, "*%d", tsz);
		for (i = 0; cmd[i]; i++) RSputc(cmd[i]);
		RSputc(ACK);
		cmd[0] = '\0';
	} else {
		if (cmd[0]) RSputs(cmd);
	}

	if (cmd[0]) Print(".. %s [%s: %d bytes]\n", cmd, buf, tsz);
	else	    Print(".. [%s: %d bytes]\n", buf, tsz);

	if ((kind & kFLLO) && cmd[0]) {
		/* ��꤫��α�����å�����(2��)���Ԥä�ɽ�� */
		for (i = 0; buf[i] = RSgetx(60); i++) {
			if (buf[i] == LF) {
				buf[i] = 0;
				Print("%s\n", buf);
				if (strstr(buf, "S-Format") ||
				    strstr(buf, "S-format")) break;
				i = -1;
			}
		}
	}

	/* �����Ԥ� */
	sleep_ms(10);

	/* �ºݤΥǡ��������� */
	if (kind & kXMODEM) {	/* XMODEM ���� */
		for (i = k = 0, sz = tsz; ; i++) {
			if ((n = sz) > XBLKSZ) n = XBLKSZ;
			if (n) {
				if (fread(&buf[3], 1, n, loadfp) != n) {
					Print(".. Can't read");
					send_abort();
					break;
				}
				sz -= n;
			}
			if (xmodem_send(i, buf, n)) break;
			if ((k += n) % (2048) == 0) {
				Putc((k % 10240) ? '.' : '*');
				TYflush();
			}
		}

	} else {	/* ̵������ */
		for (k = 0; (i = getc(loadfp)) != EOF;) {
			RSoutx(i);
			if ((++k) % 256 == 0 || i == LF) {
				sleep_ms(1);	/* ǰ�Τ����Ԥ�������� */
				if (k % 2048 == 0) {
					Putc((k % 10240) ? '.' : '*');
					TYflush();
				}
			}
		}
		RSDrain();
		sleep_ms(10);
	}

#ifdef	EVAL_TIME
	tm = time(NULL) - tm;
	if (tm <= 0) tm = 1;
	Print(" (%d.%d bytes/sec)\n", tsz / tm, (tsz % tm) * 10 / tm);
#endif

EXIT:
	if (loadfp) fclose(loadfp);
	loadfp = NULL;
	signal(SIGINT, (void*)int_break);
	return 0;
}

#else	/* USE_LOADCMD */

int	do_load(int xmodem, int memimg, int kind, uchar *p)
{
	if (!(kind & kRECV)) {
		Print("Not implemented...\n");
	}
	return -1;
}

#endif /* USE_LOADCMD */
