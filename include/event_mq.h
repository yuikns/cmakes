/*
 * event_mq.h
 *
 * Libevent threaded message passing primitives header
 *
 * Andrew Danforth <acd@weirdness.net>, October 2006
 *
 * Copyright (c) Andrew Danforth, 2006
 *
 */

#ifndef _EVENT_MSGQUEUE_H_
#define _EVENT_MSGQUEUE_H_

#include <event.h>

#ifdef __cplusplus
extern "C" {
#endif

struct event_mq;

struct event_mq *emq_new(struct event_base *, unsigned int, void (*)(void *, void *), void *);
int emq_push(struct event_mq *, void *);
unsigned int emq_length(struct event_mq *);
void emq_destroy(struct event_mq*);

#ifdef __cplusplus
}
#endif

#endif