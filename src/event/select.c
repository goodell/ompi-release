/*	$OpenBSD: select.c,v 1.2 2002/06/25 15:50:15 mickey Exp $	*/

/*
 * Copyright 2000-2002 Niels Provos <provos@citi.umich.edu>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "ompi_config.h"
#include "util/output.h"

#include <sys/types.h>
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#else
#include <sys/_time.h>
#endif
#include <sys/queue.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <err.h>
#include "mca/oob/base/base.h"

#ifdef USE_LOG
#include "log.h"
#else
#define LOG_DBG(x)
#define log_error(x)	perror(x)
#endif

#include "event.h"
#include "evsignal.h"
#include "threads/mutex.h"

extern struct ompi_event_list ompi_eventqueue;
extern ompi_mutex_t ompi_event_lock;

#ifndef howmany
#define        howmany(x, y)   (((x)+((y)-1))/(y))
#endif

extern volatile sig_atomic_t ompi_evsignal_caught;

struct selectop {
	int event_fds;		/* Highest fd in fd set */
	int event_fdsz;
	fd_set *event_readset;
	fd_set *event_writeset;
	sigset_t evsigmask;
} sop;

static void *select_init	(void);
static int select_add		(void *, struct ompi_event *);
static int select_del		(void *, struct ompi_event *);
static int select_recalc	(void *, int);
static int select_dispatch	(void *, struct timeval *);

const struct ompi_eventop ompi_selectops = {
	"select",
	select_init,
	select_add,
	select_del,
	select_recalc,
	select_dispatch
};

static void *
select_init(void)
{
	/* Disable select when this environment variable is set */
	if (getenv("EVENT_NOSELECT"))
		return (NULL);
	memset(&sop, 0, sizeof(sop));
	ompi_evsignal_init(&sop.evsigmask);
	return (&sop);
}

/*
 * Called with the highest fd that we know about.  If it is 0, completely
 * recalculate everything.
 */

static int 
select_recalc(void *arg, int max)
{
	struct selectop *sop = arg;
	fd_set *readset, *writeset;
	struct ompi_event *ev;
	int fdsz;

	if (sop->event_fds < max)
		sop->event_fds = max;

	if (!sop->event_fds) {
		TAILQ_FOREACH(ev, &ompi_eventqueue, ev_next)
			if (ev->ev_fd > sop->event_fds)
				sop->event_fds = ev->ev_fd;
	}

	fdsz = howmany(sop->event_fds + 1, NFDBITS) * sizeof(fd_mask);
	if (fdsz > sop->event_fdsz) {
		if ((readset = realloc(sop->event_readset, fdsz)) == NULL) {
			log_error("malloc");
			return (-1);
		}

		if ((writeset = realloc(sop->event_writeset, fdsz)) == NULL) {
			log_error("malloc");
			free(readset);
			return (-1);
		}

		memset((char *)readset + sop->event_fdsz, 0,
		    fdsz - sop->event_fdsz);
		memset((char *)writeset + sop->event_fdsz, 0,
		    fdsz - sop->event_fdsz);

		sop->event_readset = readset;
		sop->event_writeset = writeset;
		sop->event_fdsz = fdsz;
	}
#if OMPI_EVENT_USE_SIGNALS
	return (ompi_evsignal_recalc(&sop->evsigmask));
#else
	return (0);
#endif
}

static int
select_dispatch(void *arg, struct timeval *tv)
{
	int maxfd, res;
	struct ompi_event *ev, *next;
	struct selectop *sop = arg;

	memset(sop->event_readset, 0, sop->event_fdsz);
	memset(sop->event_writeset, 0, sop->event_fdsz);

	TAILQ_FOREACH(ev, &ompi_eventqueue, ev_next) {
		if (ev->ev_events & OMPI_EV_WRITE)
			FD_SET(ev->ev_fd, sop->event_writeset);
		if (ev->ev_events & OMPI_EV_READ)
			FD_SET(ev->ev_fd, sop->event_readset);
	}

#if OMPI_EVENT_USE_SIGNALS
	if (ompi_evsignal_deliver(&sop->evsigmask) == -1)
		return (-1);
#endif

        /* release lock while waiting in kernel */
        if(ompi_using_threads()) {
            ompi_mutex_unlock(&ompi_event_lock);
	    res = select(sop->event_fds + 1, sop->event_readset, 
	        sop->event_writeset, NULL, tv);
            ompi_mutex_lock(&ompi_event_lock);
        } else {
	    res = select(sop->event_fds + 1, sop->event_readset, 
	        sop->event_writeset, NULL, tv);
        }

#if OMPI_EVENT_USE_SIGNALS
	if (ompi_evsignal_recalc(&sop->evsigmask) == -1)
		return (-1);
#endif

	if (res == -1) {
		if (errno != EINTR) {
            ompi_output("select failed with errno=%d\n", errno);
		    return (-1);
		}

#if OMPI_EVENT_USE_SIGNALS
		ompi_evsignal_process();
#endif
		return (0);
	} 
#if OMPI_EVENT_USE_SIGNALS
	else if (ompi_evsignal_caught)
		ompi_evsignal_process();
#endif
	maxfd = 0;
	for (ev = TAILQ_FIRST(&ompi_eventqueue); ev != NULL; ev = next) {
		next = TAILQ_NEXT(ev, ev_next);

		res = 0;
		if (FD_ISSET(ev->ev_fd, sop->event_readset))
			res |= OMPI_EV_READ;
		if (FD_ISSET(ev->ev_fd, sop->event_writeset))
			res |= OMPI_EV_WRITE;
		res &= ev->ev_events;

		if (res) {
			if (!(ev->ev_events & OMPI_EV_PERSIST))
				ompi_event_del_i(ev);
			ompi_event_active_i(ev, res, 1);
		} 
        if ((ev->ev_flags & ~OMPI_EVLIST_ACTIVE) == 0 && ev->ev_fd > maxfd)
            maxfd = ev->ev_fd;
	}

	sop->event_fds = maxfd;
	return (0);
}

static int
select_add(void *arg, struct ompi_event *ev)
{
	struct selectop *sop = arg;

#if OMPI_EVENT_USE_SIGNALS
	if (ev->ev_events & OMPI_EV_SIGNAL)
		return (ompi_evsignal_add(&sop->evsigmask, ev));
#endif

	/* 
	 * Keep track of the highest fd, so that we can calculate the size
	 * of the fd_sets for select(2)
	 */
	if (sop->event_fds < ev->ev_fd)
		sop->event_fds = ev->ev_fd;

	return (0);
}

/*
 * Nothing to be done here.
 */

static int
select_del(void *arg, struct ompi_event *ev)
{
#if OMPI_EVENT_USE_SIGNALS
	struct selectop *sop = arg;

	if (!(ev->ev_events & OMPI_EV_SIGNAL))
		return (0);

	return (ompi_evsignal_del(&sop->evsigmask, ev));
#else
	return (0);
#endif
}

