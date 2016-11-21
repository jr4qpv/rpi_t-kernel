/*
	devio.c		gterm : �ǥХ��� I/O & �����ƥ��¸����

	Copyright (C) 1996-2012 by Personal Media Corporation
*/
#include	<stdio.h>
#include	<stdlib.h>
#include	<signal.h>
#include	<sys/file.h>
#include	<fcntl.h>
#include	<sys/ioctl.h>
#include	<sys/param.h>
#include	<setjmp.h>
#include	<sys/time.h>
#include	<termios.h>
#include	<sys/types.h>
#include	<unistd.h>
#include	<errno.h>

#include	<string.h>
#include	<sys/socket.h>
#include	<netinet/in.h>
#include	<netdb.h>
#include	<sys/un.h>

#include	"devio.h"
#include	"gterm.h"

#ifndef IPPORT_TELNET
#  define IPPORT_TELNET 23
#endif
static	int	tty_fd = -1;		/* ���󥽡������� FD		*/
static	int	tty_wfd;		/* ���󥽡������ FD		*/
static	struct	termios	sgtty, savetty;	/* ���󥽡�������ѥ�᡼��	*/

#define	COBUF_SZ	4096		/* ���󥽡�����ϥХåե�������	*/
static	uchar	cobuf[COBUF_SZ + 256];	/* ���󥽡�����ϥХåե�	*/
static	uchar	*co_ptr, *coe_ptr;	/* ���󥽡�����ϥХåե��ݥ��� */

static	int	rs_fd = -1;		/* ���� FD			*/

#define	LIBUF_SZ	16384		/* �������ϥХåե�������	*/
static	uchar	libuf[LIBUF_SZ + 1];	/* �������ϥХåե�(���)	*/
static	uchar	*libuf_p;		/* �������ϥХåե�PUT�ݥ���	*/
static	uchar	*libuf_g;		/* �������ϥХåե�GET�ݥ���	*/
static	uchar	*libuf_e;		/* �������ϥХåե�END�ݥ���	*/

extern	int	ignore_echo;		/* ������̵�� �⡼��		*/
static	int	sjis_mode;		/* SHIFT JIS �⡼��		*/

static	uchar	tyinpre1, tyinpre2;	/* TYin ������� 1, 2 �Х�����	*/
static	int	key_lf;			/* �������Ϥˤ����Ԥ���	*/
static	void	rs_read(void);

#define DEF_PORT	(567)		/* �ǥե���ȥݡ����ֹ�		*/
static	int	telnet;			/* �������åȤ� telnet �ξ��	*/
static	int	asyncport;		/* ��Ʊ���̿��ݡ��Ȥξ��	*/
static	int	(*openPort)(int verb);
static	int	(*closePort)(void);

/*
	������ 1 ʸ���ܤΥ����å�
*/
int	isk1(int c)
{
	return iseucknj(c);
}

/*
	������ 2 ʸ���ܤΥ����å�
*/
int	isk2(int c)
{
	return iseucknj(c);
}

/*
	���Ϥ�̵ͭ�Υ����å�
*/
static	int	in_check(int fd)
{
#ifndef	TIOCINQ
#define	TIOCINQ	FIONREAD
#endif

#ifdef __CYGWIN__
	/* !! Cygwin �Ǥϡ�ioctl(rs_fd, TIOCINQ, &len) ��������ư��ʤ�
	     (�Ť��С�����󡢲���ü��)���Ȥ����뤿�ᡢselect() ����� !! */
	fd_set	fds;
	struct	timeval tv;

	FD_ZERO(&fds);
	FD_SET(fd, &fds);

	tv.tv_sec = 0;		/* �Ԥ��ʤ� */
	tv.tv_usec = 0;

	select(fd + 1, &fds, NULL, NULL, &tv);
	return (FD_ISSET(fd, &fds)) ? 1: 0;
#else
	int	nc;

	ioctl(fd, TIOCINQ, &nc);
	return nc;
#endif
}

/*
	���󥽡���(TTY) �ν����
*/
int	TYinit(int use_stdin, int arg_ign_echo)
{
	FILE	*ttyfp;

	ignore_echo = arg_ign_echo;

	if (use_stdin) {
		tty_fd = STDIN_FILENO;
		tty_wfd = STDOUT_FILENO;

	} else if (ttyfp = fopen("/dev/tty", "r+")) {	/* raw i/o */
		tty_wfd = tty_fd = fileno(ttyfp);
	}

	if (tty_fd >= 0) {
		tcgetattr(tty_fd, &savetty);
		sgtty = savetty;
		sgtty.c_lflag &= ~(ICANON | ECHO);
		sgtty.c_cc[VMIN] = 0;
		sgtty.c_cc[VEOF] = 0;
		sgtty.c_iflag &= ~(INLCR | ICRNL | ISTRIP | IXON | BRKINT);
		sgtty.c_oflag &= ~(OPOST);
		tcsetattr(tty_fd, TCSADRAIN, &sgtty);
		co_ptr = cobuf;
		coe_ptr = co_ptr + COBUF_SZ - 1;
		return 0;
	}
	return 1;
}

/*
	���󥽡���(TTY) �κƽ����
*/
void	TYreinit(void)
{
	tyinpre1 = tyinpre2 = 0;
	tcsetattr(tty_fd, TCSADRAIN, &sgtty);
}

/*
	���󥽡���(TTY) �ν��ϥե�å���
*/
void	TYflush(void)
{
	uchar	*ptr;
	uchar	*ps;
	uchar	*pknj;
	int	i, d_size, d_sz, m_sz;
	int	i1, i2;

	if (co_ptr - cobuf) {
		if (ignore_echo) {	/* eclipse ���󥽡����б�	*/
			for (ptr = cobuf; ptr < co_ptr; ptr++) {
				if (*ptr == CR) {
					memmove(ptr, ptr+1, co_ptr - (ptr+1));
					co_ptr -= 1;
				}
			}
		}
	}
	if (co_ptr - cobuf) {

		rs_read();		/* ���������ɤ߹��� */
		for (pknj = cobuf; pknj < co_ptr; pknj++) {
			if (isk1(*pknj)) {
				if (pknj + 1 >=  co_ptr)
					break;
				if (isk2(*(pknj+1))) {
					pknj++;
				}
			}
		}
		d_sz = d_size = pknj - cobuf;
		if (sjis_mode) {	/* EUC->SJIS �Ѵ�		*/
			for (i = 0; i < d_sz; i++) {
				if (isk1(cobuf[i])
					&& (i + 1 < d_sz)
					&& isk2(cobuf[i+1])) {
					i1 = cobuf[i]; i2 = cobuf[i+1];
					euctosj(&i1, &i2);
					cobuf[i] = i1; cobuf[i+1] = i2;
					i++;
				}
			}
		}

		for( ptr = cobuf; d_size > 0 ; ptr += i, d_size -= i ) {
			i = write(tty_wfd, ptr, d_size);
			if (i < 0) break;
		}
		if (d_sz > 0) {
			m_sz = co_ptr - cobuf - d_sz;
			if (m_sz > 0) {
				memmove(cobuf, cobuf + d_sz, m_sz);
			}
			co_ptr = cobuf + m_sz;
		}
	}
}

/*
	���󥽡���(TTY) �ν��ϥХåե����ꥢ
*/
void	TYclr(void)
{
	co_ptr = cobuf;
}

/*
	���󥽡���(TTY) �ؤ� 1 ʸ������
*/
void	TYout(uchar c)
{
	if (c == '\n'){
		*co_ptr++ = CR;
		*co_ptr++ = c;
		TYflush();
	} else {
		*co_ptr++ = c;
		if (co_ptr >= coe_ptr) TYflush();
	}
}

/*
	���󥽡���(TTY) �ؤ�ʸ�������
*/
void	TYouts(uchar *s)
{
	uchar	c;

	while (c = *s++) {
		if (c == '\n') *co_ptr++ = CR;
		*co_ptr++ = c;
	}
	TYflush();
}

/*
	���󥽡���(TTY) ����� 1 ʸ������

	Linux/Cygwin(���ޥ�ɥץ��ץ�)/Cygwin(Cygterm)/eclipse �γƥ��󥽡���
	�ˤ�����[Enter]�������б����륳���ɤ��ۤʤ롣
		Linux				0x0d(\r:CR)
		Cygwin(���ޥ�ɥץ��ץ�)	0x0d(\r:CR)
		Cygwin(Cygterm)			0x0d(\r:CR)
		eclipse				0x0d,0x0a(\r\n:CRLF)
	���Τ��ᡢignore_echo ��eclipse �ѤȤߤʤ���LF �����롣
*/
uchar	TYin(void)
{
	uchar	c1, c2;
	int	i1, i2;

	c1 = 0;
	rs_read();	/* ���������ɤ߹��� */

	if (tyinpre2) {
		c1 = tyinpre2;	tyinpre2 = 0; goto dir_ret;
	}
	if (tyinpre1) {
		c1 = tyinpre1; tyinpre1 = 0;
	} else {
		if (in_check(tty_fd) <= 0) {
			goto dir_ret;	/* ���Ϥʤ�?			*/
		} if (read(tty_fd, &c1, 1) != 1) {
			goto dir_ret;	/* �����Բ�			*/
		}
		if (ignore_echo) {
			if (c1 != CR) {
				if (c1 == LF) key_lf = 1;
			} else if ((in_check(tty_fd) > 0)
					&& (read(tty_fd, &c2, 1) == 1)) {
					if (c2 != LF) {
						tyinpre1 = c2;
					} else {
						key_lf = 1;
					}
			}
		}
	}
	if (sjis_mode && issjisk1(c1)) {
		sleep_ms(10);
		if ((in_check(tty_fd) > 0)
			&& (read(tty_fd, &c2, 1) == 1)) {
			if (issjisk2(c2)) {
				i1 = c1; i2 = c2;
				sjtoeuc(&i1, &i2);
				c1 = (uchar)i1;
				tyinpre2 = i2;
			} else {
				tyinpre1 = c2;
			}
		}
	}
dir_ret:
	return c1;
}

/*
	�������ϥ����ɤΥ����å�
*/
int	Keytest(uchar c)
{
	uchar	c1;
	int	i;

#define	ESC_WAIT	sleep_ms(50)

	if (c == ESC) {
		for (i = 4; --i >= 0 && (c1 = TYin()) == 0; ESC_WAIT);

		if (c1 == '[') {
			for (i = 4; --i >= 0 && (c1 = TYin()) == 0; ESC_WAIT);

			switch (c1) {
			case 'A': return CUR_UP;
			case 'B': return CUR_DWN;
			case 'C': return CUR_FWD;
			case 'D': return CUR_BWD;
			case '2':
				for (i = 0; (c1 = TYin()) != 'z';) {
					if (c1) i = i * 10 + c1 - '0';
				}
				if (i == 9)  return CUR_UPx;
				if (i == 12) return CUR_DWNx;
				if (i == 13) return CUR_FWDx;
				if (i == 11) return CUR_BWDx;
			}
			return 0;		/* ignore */
		}
	}
	return c;
}

/*
	�����Хåե��ݥ��󥿤ν����
*/
static	void	rs_reset_ptr(void)
{
	libuf_p = libuf_g = libuf;
	libuf_e = libuf_p + LIBUF_SZ;
}

/*
	�ܡ��졼�ȤΥ����å�
*/
int	checkBaud(int baud)
{
	switch (baud) {		/* �ܡ��졼�� */
	case 50:	baud = B50;	break;
	case 75:	baud = B75;	break;
	case 110:	baud = B110;	break;
	case 134:	baud = B134;	break;
	case 150:	baud = B150;	break;
	case 200:	baud = B200;	break;
	case 300:	baud = B300;	break;
	case 600:	baud = B600;	break;
	case 1200:	baud = B1200;	break;
	case 1800:	baud = B1800;	break;
	case 2400:	baud = B2400;	break;
	case 4800:	baud = B4800;	break;
	case 9600:	baud = B9600;	break;
	case 19200:	baud = B19200;	break;
	case 38400:	baud = B38400;	break;
	case 57600:	baud = B57600;	break;
	case 115200:	baud = B115200; break;
#ifdef	B230400
	case 230400:	baud = B230400; break;
#endif
#ifdef	B460800
	case 460800:	baud = B460800; break;
#endif
#ifdef	B921600
	case 921600:	baud = B921600; break;
#endif
#ifdef	B76800
	case 76800:	baud = B76800;	break;
#endif
#ifdef	B153600
	case 153600:	baud = B153600; break;
#endif
#ifdef	B307200
	case 307200:	baud = B307200; break;
#endif
#ifdef	B576000
	case 576000:	baud = B576000; break;
#endif
#ifdef	B1152000
	case 1152000:	baud = B1152000; break;
#endif
#ifdef	B500000
	case 500000:	baud = B500000; break;
#endif
#ifdef	B1000000
	case 1000000:	baud = B1000000; break;
#endif
#ifdef	B1500000
	case 1500000:	baud = B1500000; break;
#endif
#ifdef	B2000000
	case 2000000:	baud = B2000000; break;
#endif
#ifdef	B2500000
	case 2500000:	baud = B2500000; break;
#endif
#ifdef	B3000000
	case 3000000:	baud = B3000000; break;
#endif
#ifdef	B3500000
	case 3500000:	baud = B3500000; break;
#endif
#ifdef	B4000000
	case 4000000:	baud = B4000000; break;
#endif
	default:	return -1;
	}
	return baud;
}

static struct	hostent		*hptr;
static struct	sockaddr_in	nam;

/*
	�����åȤμ¥����ץ�
*/
static int SOopen(int verb)
{
	int ret = 0;
	
	rs_fd = socket(PF_INET, SOCK_STREAM, 0);
	if (rs_fd == -1) {
		if (verb) TYouts(strerror(errno));
		ret = -1; goto err_ret;
	}
	
	ret = connect(rs_fd, (struct sockaddr *)&nam, sizeof(nam));
	if (ret == -1) {
		if (verb) TYouts(strerror(errno));
		close(rs_fd);
		rs_fd = -1;
	}
err_ret:
	return ret;
}

/*
	�����åȤμ¥�����
*/
static int SOclose(void)
{
	int ret;
	
	if (rs_fd == -1) {
		ret = -1; goto err_ret;
	}
	ret = close(rs_fd);
	rs_fd = -1;

err_ret:
	return ret;
}

/*
	�����åȤΥ����ץ�
*/
int	SOinit(uchar *line)
{
	char	*p;
	char	*tmp = NULL;
	char	*pp = NULL;
	int	ret = 0, port = DEF_PORT;

	if (line == NULL) {
		TYouts("No Address error\n");
		ret = -1; goto err_ret;
	}
	
	tmp = strdup(line);
	if (tmp == NULL) {
		TYouts(strerror(ENOMEM));
		TYouts("\n");
		ret = -1; goto err_ret;
	}
				/* xxxx:yyy ������ʬΥ	*/	
	p = strchr(tmp, ':');
	if (p) {
		*p++ = 0;
		pp = p;
		for (; *p; p++) {
			if (!isdigit(*p)) {
				TYouts("xxxx:yyy format error");
				TYouts("\n");
				ret = -1; goto err_ret;
			}
		}
		if (p - pp > 0) {
			sscanf(pp, "%d", &port);
			if (port == IPPORT_TELNET)
				telnet = 1;
		}
	}
	hptr = gethostbyname(tmp);
	if (hptr == NULL) {
		TYouts(strerror(errno));
		TYouts("\n");
		ret = -1; goto err_ret;
	}

	rs_reset_ptr();

	memset(&nam, 0, sizeof(nam));
	nam.sin_family = hptr->h_addrtype;	/* AF_INET/AF_INET6	*/
	nam.sin_port = htons(port);
	memcpy(&nam.sin_addr, hptr->h_addr, hptr->h_length);

	openPort = SOopen;
	closePort = SOclose;
	ret = (*openPort)(1);
	if (ret == -1) {
		TYouts(".. Socket INIT error\n");
	}

err_ret:
	if (tmp) free(tmp);
	return ret;
}

static 	struct sockaddr_un	srvaddr;

/*
	UNIX�ɥᥤ�󥽥��åȤμ¥����ץ�
*/
static int	SOlocalopen(int verb)
{
	int ret;

	rs_fd = socket(AF_LOCAL, SOCK_STREAM, 0);
	if (rs_fd == -1) {
		if (verb) TYouts(strerror(errno));
		ret = -1; goto err_ret;
	}
	ret = connect(rs_fd, (struct sockaddr *)&srvaddr, sizeof(srvaddr));
	if (ret == -1) {
		if (verb) TYouts(strerror(errno));
		close(rs_fd);
		rs_fd = -1;
	}
err_ret:
	return ret;
}

/*
	UNIX�ɥᥤ�󥽥��åȤΥ����ץ�
*/
int	SOlocalinit(uchar *line)
{
	int ret;

	if (line == NULL) {
		TYouts("No device error\n");
		ret = -1; goto err_ret;
	}
	rs_reset_ptr();

	memset(&srvaddr, 0, sizeof(srvaddr));
	srvaddr.sun_family = AF_LOCAL;
	strncpy(srvaddr.sun_path, line, sizeof(srvaddr.sun_path)-1);

	openPort = SOlocalopen;
	closePort = SOclose;
	ret = (*openPort)(1);
	if (ret == -1) {
		TYouts(".. Socket INIT error\n");
	}
err_ret:
	return ret;
}

/*
	ʸ�������ɤ�����
*/
void	SetCode(int sjis)
{
	sjis_mode = sjis;		/* SHIFT JIS �⡼�� */
}

struct	termios	rs_arg_org, rs_arg;
int	clen, par, sbit, noflow, baud;
uchar	*devnam;

/*
	�����μ¥����ץ�
*/
int	RSopen(int verb)
{
	int ret = 0;

	if (devnam == NULL) {
		if (verb) TYouts("No device error");
		ret = -1;	goto err_ret;
	}

	if ((rs_fd = open(devnam, O_RDWR, 0777)) < 0) {
		if (verb) TYouts(strerror(errno));
		ret = -1;	goto err_ret;
	}

	/* �����ѥ�᡼������ */
	if (tcgetattr(rs_fd, &rs_arg_org) == -1) {
		if (verb) TYouts(strerror(errno));
		ret = -1;	goto err_ret;
	}
	
	rs_arg = rs_arg_org;

	/* �����ѥ�᡼������ */
	cfsetospeed(&rs_arg, baud);
	cfsetispeed(&rs_arg, 0);

	rs_arg.c_cflag &= ~(CRTSCTS | CSIZE | CSTOPB | PARENB | PARODD);
	rs_arg.c_cflag |= (noflow ? 0 : CRTSCTS)
				| clen | sbit | par | CREAD | CLOCAL;
	rs_arg.c_lflag = 0;
	rs_arg.c_cc[VTIME] = 0;
	rs_arg.c_cc[VMIN] = 1;
	rs_arg.c_iflag = IGNPAR;
	rs_arg.c_oflag = 0;
	tcflush(rs_fd, TCIFLUSH);
	if (tcsetattr(rs_fd, TCSANOW, &rs_arg) == -1) {
		if (verb) TYouts(strerror(errno));
		ret = -1;	goto err_ret;
	}
	asyncport = 1;
err_ret:
	if (ret == -1 && rs_fd != -1) {
		close(rs_fd);
		rs_fd = -1;
	}
	return ret;
}

/*
	�����μ¥�����
*/
int	RSclose(void)
{
	int ret;

	asyncport = 0;
	if (rs_fd == -1) {
		ret = -1;	goto err_ret;
	}
	tcflush(rs_fd, TCIOFLUSH);
	tcsetattr(rs_fd, TCSANOW, &rs_arg_org);
	ret = close(rs_fd);
	rs_fd = -1;
err_ret:
	return ret;
}

/*
	�����ν����
*/
int	RSinit(uchar *line, int arg_clen, int arg_par, int arg_sbit,
					int arg_noflow, int arg_baud)
{
	int	arg, ret = 0;

	noflow = arg_noflow;
	switch (arg_clen) {		/* �ǡ���Ĺ�� */
	case 5: 	clen = CS5;	break;
	case 6: 	clen = CS6;	break;
	case 7: 	clen = CS7;	break;
	case 8: 
	default:	clen = CS8;
	}

	switch (arg_par) {		/* �ѥ�ƥ� */
	case 1: 	par = PARENB | PARODD;	break;
	case 2: 	par = PARENB;		break;
	case 0:
	default:	par = 0;
	}

	switch (arg_sbit) {		/* ���ȥåץӥå� */
	case 1: 	sbit = CSTOPB;	break;
	case 0:
	default:	sbit = 0;
	}

	baud = checkBaud(arg_baud);	/* �ܡ��졼�� */
	if (baud < 0) baud = B19200;

	devnam = strdup(line);
	if (devnam == NULL) {
		TYouts(strerror(errno));
		TYouts(".. RS INIT error\n");
		ret = -1; goto err_ret;
	}

	rs_reset_ptr();
	
	openPort = RSopen;
	closePort = RSclose;
	ret = (*openPort)(1);
	if (ret == -1) {
		TYouts(".. RS INIT error\n");
	}
err_ret:
	return ret;
}

/*
	�����Υ��ꥢ
*/
void	RSclr(void)
{
	if (asyncport) tcflush(rs_fd, TCIOFLUSH);
	rs_reset_ptr();
}

/*
	����������ɤ߹���(�ݡ����)
*/
static	void	rs_read(void)
{
	int	cnt, sz1, sz2;

	if (rs_fd < 0) return;

	/* !! Cygwin �Ǥϡ�in_check() �� select() �˻��֤������뤿��
	      �����Хåե���ʸ�������äƤ���������Ϥ��ʤ� !! */

	if (libuf_p != libuf_g) return;		/* �������Ϻ� */

	/* ���Ϥ�̵ͭ�Υ����å� */
	if (in_check(rs_fd) <= 0) return;	/* ���Ϥʤ� */

	/* �����Хåե��ζ��������� */
	if (libuf_g > libuf_p) {
		sz1 = libuf_g - libuf_p - 1;
		sz2 = 0;
	} else {
		sz1 = libuf_e - libuf_p;
		sz2 = libuf_g - libuf;
		if (sz2 > 0) sz2--;
		else	     sz1--;
	}

	/* �ɤ߹��� */
	cnt = read(rs_fd, libuf_p, sz1);
	if (cnt <= 0) return;
	if ((libuf_p += cnt) >= libuf_e) libuf_p = libuf;

	if (cnt >= sz1 && sz2 > 0) {
		cnt = read(rs_fd, libuf_p, sz2);
		if (cnt > 0) {
			if ((libuf_p += cnt) >= libuf_e) libuf_p = libuf;
		}
	}
}

/*
	��������� 1 ʸ������
	�����:	-1	���Ϥʤ�
		>= 0	����ʸ��
*/
int	RSin(void)
{
	int	c, c2, i;

	rs_read();		/* ���������ɤ߹��� */

	if (libuf_g == libuf_p) return -1;	/* ���Ϥʤ� */

	c = *libuf_g;
	if (++libuf_g >= libuf_e) libuf_g = libuf;

	return c;
}

/*
	�����ؤ� 1 ʸ��ľ�ܽ���
*/
void	RSoutx(uchar c)
{
	while (write(rs_fd, &c, 1) != 1);
}

/*
	�����ؤ� 1 ʸ������
*/
void	RSout(uchar c)
{
	RSoutx(c);			/* ���� */
	rs_read();			/* ���������ɤ߹��� */
}

/*
	�����ؤ�¿�Х��Ƚ���
*/
void	RSwrite(uchar *buf, int len)
{
extern	int	out_wait;

	if (out_wait <= 0) {
		write(rs_fd, buf, len);
	} else {
		while (--len >= 0) {
			RSoutx(*buf++);
			if (out_wait > 0 && (len & 0x07) == 0)
				sleep_us(out_wait / 2);
		}
	}
}

/*
	�����ؤ� 1 ʸ������
		uchar c		����ʸ��
*/
void	RSputc(uchar c)
{
extern	int	out_wait;
	RSout(c);
	if (out_wait > 0) sleep_us(out_wait);	/* ʸ�����ϸ���Ԥ� */
}

/*
	�����ؤ�ʸ�������

	ʸ���������˲��ԥ����ɤ��ɲä���
	eclipse �Υ��󥽡���ϡ�ü�����椬����ʤ��Τǡ���Enter�פ����Ϥ���
	�ȡ��ȼ��˲��Ԥ��Ƥ��ޤ������Τ��� �������åȵ��狼��Υ������Хå�
	������ 0x0d, 0x0a ������̵�뤷�ʤ��ȡ�0x0a �ˤ����Ԥ�ȯ�����Ƥ�
	�ޤ�����������������ץȤˤ�륳�ޥ�ɼ¹Ԥξ��� 0x0a ���֤�ɬ��
	�����롣
*/
void	RSputs(uchar *s)
{
	int len;
	char buf[LINE_LEN+2];

	if (telnet) {				/* �������åȤ� telnet �ξ��*/
		len = strlen(s);
		memcpy(buf, s, len);
		buf[len] = CR; buf[len + 1] = LF;
		RSwrite(buf, len + 2);
	} else {
		while (*s) RSputc(*s++);
		RSout(CR);
		if (ignore_echo && key_lf) {
			while (RSgetc(0, -1) != LF);
			key_lf = 0;
		} else
			while (RSgetc(1, -1) != CR);	/* ��������̵�� */
	}
}

/*
	rs_fd ���ԤäƤ�����Ϥ�ž������λ����ޤ��Ԥ�
*/
void RSDrain()
{
	if (asyncport) tcdrain(rs_fd);
}

/*
	�ץ���ཪλ
*/
void int_exit(void)
{
	sys_end(0);
}

/*
	���� or ���󥽡���������Ԥ�
	��  ��: int tmout	>= 0	�����ॢ����(�ߥ���)
				-1	�����ॢ���Ȥʤ�
	�����:	| 0x1			TTY ���Ϥ���
		| 0x2			�������Ϥ���
		0			�����ॢ����(������TTY �������Ϥʤ�)
*/
int	PollRSTY(int tmout)
{
	fd_set	fdmask;
	int	maxfd;
	int	ret = 0;
	struct	timeval tv;
	struct	timeval *ptv;

	TYflush();
	if (libuf_p != libuf_g) return 0x2;	/* �������Ϻ� */

	if ((maxfd = rs_fd) < tty_fd) maxfd = tty_fd;

	while(1) {
		FD_ZERO(&fdmask);
		FD_SET(rs_fd, &fdmask);
		FD_SET(tty_fd, &fdmask);

		if (tmout < 0) {
			ptv = NULL;
		} else {
			ptv = &tv;
			tv.tv_sec = tmout / 1000;
			tv.tv_usec = (tmout - tv.tv_sec * 1000) * 1000;
		}
				/* �ɤ��餫�������ϤǤ���ޤ��Ԥ� */
		ret = select(maxfd + 1, &fdmask, NULL, NULL, ptv);
#ifdef __CYGWIN__
		if (ret > 0 && asyncport == 0 && FD_ISSET(rs_fd, &fdmask)) {
#else /* !__CYGWIN__ */
		if (ret > 0 && FD_ISSET(rs_fd, &fdmask)) {
#endif /* !__CYGWIN__ */

			int	nc;
			ret = ioctl(rs_fd, FIONREAD, &nc);
			if (ret == 0 && nc == 0) ret = -1;
		}
		if (ret == -1) {
			if (closePort) (*closePort)();
			signal(SIGINT, (void*)int_exit);
			while (rs_fd == -1) {
				sleep_ms(500);
				(*openPort)(0);
			}
			signal(SIGINT, (void*)int_break);
			continue;
		}
		break;
	}

	if (FD_ISSET(rs_fd, &fdmask)) {
		ret |= 0x2;
		rs_read();			/* ���������ɤ߹��� */
	}
	if (FD_ISSET(tty_fd, &fdmask)) {	/* ���󥽡������Ϥ�̵ͭ */
		ret |= 0x1;
	}
	return ret;
}

/*
	��������ΰ�������
*/
void	cur_addr(int l, int c)
{
	char	buf[20];

	sprintf(buf, "\033[%d;%dH", l + 1, c + 1);
	TYouts(buf);
}

/*
	�����ƥ�����¹�
*/
int	do_system(char *s)
{
	int	er;

	TYflush();
	tcsetattr(tty_fd, TCSADRAIN, &savetty);
	er = system(s);
	TYreinit();
	return er;
}

/*
	������¹�
*/
int	escape(void)
{
	int	er;
	char	*s;
extern	char	*getenv();

	Puts("<< break GTERM >>\n");
	s = getenv("SHELL");
	er = do_system(s ? s : DEF_SHELL);
	Puts("<< resume GTERM >>\n");
	return er;
}

/*
	�����ƥཪλ
*/
void	sys_end(int i)
{
	TYflush();

	if (tty_fd >= 0) {
		tcsetattr(tty_fd, TCSADRAIN, &savetty);
	}
	signal(SIGINT, SIG_DFL);

	if (closePort) (*closePort)();
	exit(i);
}

/*
	�ߥ��ä��Ԥ�
*/
void	sleep_ms(int n)
{
	usleep (n * 1000);
}

/*
	�ޥ������ä��Ԥ� (���ΤǤʤ�)
*/
int	dmy_cnt;
void	dmy_call(int cnt)
{
	while (--cnt >= 0) dmy_cnt++;
}
void	sleep_us(int n)
{
	int	m;

	if ((m = n / 1000) > 0) usleep (m * 1000);
	for (n %= 1000; --n >= 0; ) dmy_call(100);	/* Ŭ�����Ԥ� */
}
