/*
	sym.c		gterm : ����ܥ�ơ��֥����

	Copyright (C) 1996-2007	by Personal Media Corporation
*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "gterm.h"

/*
	����ܥ륨��ȥ����
*/
#define	L_SYMNM		160
typedef struct	{
	unsigned int	val;			/* ����ܥ���		*/
	char		nm[L_SYMNM + 1];	/* ����ܥ�ʸ����	*/
} ENT;
#define	SZ_ENT(n)	((n) * sizeof(ENT))

static	ENT	*syment = NULL;			/* ����ܥ륨��ȥ�	*/
static	int	nsyms = 0;			/* ͭ������ܥ��	*/
static	int	maxsyms = 0;			/* ���祷��ܥ��	*/

#define	ADD_SYMS	1000			/* �ɲ�ñ��		*/

/*
	qsort ����Ӵؿ�
*/
static	int	compare(unsigned int *i, unsigned int *j)
{
	return	*i - *j;
}

/*
	�ޥåץե����뤫�饷��ܥ���ɤ߹���
*/
static	void	symload_map(FILE *fp, char *fn, unsigned int base)
{
	char	buf[512], type;
	ENT	*ep;
	int	len;

	while (fgets(buf, 512, fp) != NULL) {
		if ((len = strlen(buf)) < 12) continue;
		if (buf[len - 1] == '\n') buf[len - 1] = '\0';

		/* ����ܥ륿����: B, D, T �Τ� */
		type = buf[9];
		if (type != 'B' && type != 'D' && type != 'T') continue;

		/* ������� */
		if (nsyms >= maxsyms) {
			maxsyms += ADD_SYMS;
			ep = (syment)? (ENT*)realloc(syment, SZ_ENT(maxsyms))
				     : (ENT*)malloc(SZ_ENT(maxsyms));
			if (! ep) {
				Print("Can't alloc memory\n");
				maxsyms -= ADD_SYMS;
				break;
			}
			syment = ep;
		}
		ep = syment + nsyms;
		sscanf(buf, "%08x", &ep->val);
		ep->val += base;
		strncpy(ep->nm, &buf[11], L_SYMNM);
		ep->nm[L_SYMNM] = '\0';
		nsyms++;
	}
	if (nsyms > 0) {
		/* ���ɥ쥹��˥����Ȥ��� */
		qsort(syment, nsyms, SZ_ENT(1), (void*)compare);
		Print("SYMT : %s (map) %d symbols)\n", fn, nsyms);
	}
}
/*
	����ܥ�Υե�����(*.map)����Υ���
*/
void	symload(char *fn, char *p)
{
	unsigned int	base;
	FILE		*fp;
	char		fname[512], *cp;

	if ((cp = (char*)strrchr(fn, '.')) == NULL) {
		strcpy(fname, fn);
		strcat(fname, ".map");
		fn = fname;

	} else if (strcmp(cp, ".map") != 0) {
		Print("%s : not xxx.map file\n", fn);
		return;
	}

	if ((fp = fopen(fn, "r")) == NULL) {
		Print("%s : can't open\n", fn);
		return;
	}

	if (syment) {
		free(syment);
		syment = NULL;
	}
	nsyms = maxsyms = 0;

	base = (*p) ? strtol(p, 0, 16) : 0;

	symload_map(fp, fn, base);

	fclose(fp);
}
/*
	ʸ���󤬴ޤޤ�Ƥ���ɤ����Υ����å�
*/
static	int	str_check(char *line, char *nm, int len)
{
	int	n;

	for (n = strlen (line) - len; n-- >= 0; line++) {
		if (memcmp(line, nm, len) == 0) return 1;
	}
	return 0;
}
/*
	����ܥ��ɾ�� (.@ ���ޥ��)
*/
void	symeval(char *sym, char *p)
{
	char	c;
	int	i, k;
	ENT	*ep;
	unsigned int	n;

	if (! syment || nsyms <= 0) return;

	if (isdigit(sym[0])) {
		n = strtol(sym, 0, 16);
		for (ep = syment, i = 0; i < nsyms; i++, ep++) {
			if (n < ep->val) break;
		}
		if (i) --ep;
		Print("%08x : %s + %x\n", n, ep->nm, n - ep->val);

	} else if (sym[0] == '?') {
		n = strtol(&sym[1], 0, 16);
		for (ep = syment, i = 0; i < nsyms; i++, ep++) {
			if (n < ep->val) break;
		}
		for (k = 0; k < nsyms - i; ep++) {
			Print("%08x : %s\n", ep->val, ep->nm);
			if ((++k) % 32 == 0) {
				Print("*** more (y/n) ?");
				while ((c = TYin()) == 0);
				Print("\n");
				if (c != 'y' && c != 'Y') break;
			}
		}

	} else if (n = strlen(sym)) {
		for (ep = syment, i = 0; i < nsyms; i++, ep++)
			if (str_check(ep->nm, sym, n))
				Print("%08x: %s\n", ep->val, ep->nm);
	}
}
