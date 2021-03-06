/*
 * No copyright is claimed.  This code is in the public domain; do with
 * it what you wish.
 *
 * Written by Karel Zak <kzak@redhat.com>
 */

#pragma once

#define UL_TTY_KEEPCFLAGS	(1 << 1)
#define UL_TTY_UTF8		(1 << 2)

static inline void reset_virtual_console(struct termios *tp, int flags)
{
	/* Use defaults of <sys/ttydefaults.h> for base settings */
	tp->c_iflag |= TTYDEF_IFLAG;
	tp->c_oflag |= TTYDEF_OFLAG;
	tp->c_lflag |= TTYDEF_LFLAG;

	if ((flags & UL_TTY_KEEPCFLAGS) == 0) {
#ifdef CBAUD
		tp->c_lflag &= ~CBAUD;
#endif
		tp->c_cflag |= (B38400 | TTYDEF_CFLAG);
	}

	/* Sane setting, allow eight bit characters, no carriage return delay
	 * the same result as `stty sane cr0 pass8'
	 */
	tp->c_iflag |=  (BRKINT | ICRNL | IMAXBEL);
	tp->c_iflag &= ~(IGNBRK | INLCR | IGNCR | IXOFF | IUCLC | IXANY | ISTRIP);
	tp->c_oflag |=  (OPOST | ONLCR | NL0 | CR0 | TAB0 | BS0 | VT0 | FF0);
	tp->c_oflag &= ~(OLCUC | OCRNL | ONOCR | ONLRET | OFILL | \
			    NLDLY|CRDLY|TABDLY|BSDLY|VTDLY|FFDLY);
	tp->c_lflag |=  (ISIG | ICANON | IEXTEN | ECHO|ECHOE|ECHOK|ECHOKE|ECHOCTL);
	tp->c_lflag &= ~(ECHONL|ECHOPRT | NOFLSH | TOSTOP);

	if ((flags & UL_TTY_KEEPCFLAGS) == 0) {
		tp->c_cflag |=  (CREAD | CS8 | HUPCL);
		tp->c_cflag &= ~(PARODD | PARENB);
	}
#ifdef OFDEL
	tp->c_oflag &= ~OFDEL;
#endif
#ifdef XCASE
	tp->c_lflag &= ~XCASE;
#endif
#ifdef IUTF8
	if (flags & UL_TTY_UTF8)
		tp->c_iflag |= IUTF8;	    /* Set UTF-8 input flag */
	else
		tp->c_iflag &= ~IUTF8;
#endif
	/* VTIME and VMIN can overlap with VEOF and VEOL since they are
	 * only used for non-canonical mode. We just set the at the
	 * beginning, so nothing bad should happen.
	 */
	tp->c_cc[VTIME]    = 0;
	tp->c_cc[VMIN]     = 1;
	tp->c_cc[VINTR]    = CINTR;
	tp->c_cc[VQUIT]    = CQUIT;
	tp->c_cc[VERASE]   = CERASE; /* ASCII DEL (0177) */
	tp->c_cc[VKILL]    = CKILL;
	tp->c_cc[VEOF]     = CEOF;
#ifdef VSWTC
	tp->c_cc[VSWTC]    = _POSIX_VDISABLE;
#elif defined(VSWTCH)
	tp->c_cc[VSWTCH]   = _POSIX_VDISABLE;
#endif
	tp->c_cc[VSTART]   = CSTART;
	tp->c_cc[VSTOP]    = CSTOP;
	tp->c_cc[VSUSP]    = CSUSP;
	tp->c_cc[VEOL]     = _POSIX_VDISABLE;
	tp->c_cc[VREPRINT] = CREPRINT;
	tp->c_cc[VDISCARD] = CDISCARD;
	tp->c_cc[VWERASE]  = CWERASE;
	tp->c_cc[VLNEXT]   = CLNEXT;
	tp->c_cc[VEOL2]    = _POSIX_VDISABLE;
}
