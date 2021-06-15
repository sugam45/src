/*	$NetBSD: timer_test.c,v 1.7 2021/04/29 17:26:13 christos Exp $	*/

/*
 * Copyright (C) Internet Systems Consortium, Inc. ("ISC")
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, you can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * See the COPYRIGHT file distributed with this work for additional
 * information regarding copyright ownership.
 */

#if HAVE_CMOCKA

#include <inttypes.h>
#include <sched.h> /* IWYU pragma: keep */
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define UNIT_TESTING
#include <cmocka.h>

#include <isc/atomic.h>
#include <isc/commandline.h>
#include <isc/condition.h>
#include <isc/mem.h>
#include <isc/platform.h>
#include <isc/print.h>
#include <isc/task.h>
#include <isc/time.h>
#include <isc/timer.h>
#include <isc/util.h>

#include "../timer.c"
#include "isctest.h"

/* Set to true (or use -v option) for verbose output */
static bool verbose = false;

#define FUDGE_SECONDS	  0	    /* in absence of clock_getres() */
#define FUDGE_NANOSECONDS 500000000 /* in absence of clock_getres() */

static isc_timer_t *timer = NULL;
static isc_condition_t cv;
static isc_mutex_t mx;
static isc_time_t endtime;
static isc_mutex_t lasttime_mx;
static isc_time_t lasttime;
static int seconds;
static int nanoseconds;
static atomic_int_fast32_t eventcnt;
static atomic_uint_fast32_t errcnt;
static int nevents;

static int
_setup(void **state) {
	isc_result_t result;

	UNUSED(state);

	/* Timer tests require two worker threads */
	result = isc_test_begin(NULL, true, 2);
	assert_int_equal(result, ISC_R_SUCCESS);

	atomic_init(&errcnt, ISC_R_SUCCESS);

	return (0);
}

static int
_teardown(void **state) {
	UNUSED(state);

	isc_test_end();

	return (0);
}

static void
shutdown(isc_task_t *task, isc_event_t *event) {
	isc_result_t result;

	UNUSED(task);

	/*
	 * Signal shutdown processing complete.
	 */
	result = isc_mutex_lock(&mx);
	assert_int_equal(result, ISC_R_SUCCESS);

	result = isc_condition_signal(&cv);
	assert_int_equal(result, ISC_R_SUCCESS);

	result = isc_mutex_unlock(&mx);
	assert_int_equal(result, ISC_R_SUCCESS);

	isc_event_free(&event);
}

static void
setup_test(isc_timertype_t timertype, isc_time_t *expires,
	   isc_interval_t *interval,
	   void (*action)(isc_task_t *, isc_event_t *)) {
	isc_result_t result;
	isc_task_t *task = NULL;
	isc_time_settoepoch(&endtime);
	atomic_init(&eventcnt, 0);

	isc_mutex_init(&mx);
	isc_mutex_init(&lasttime_mx);

	isc_condition_init(&cv);

	LOCK(&mx);

	result = isc_task_create(taskmgr, 0, &task);
	assert_int_equal(result, ISC_R_SUCCESS);

	result = isc_task_onshutdown(task, shutdown, NULL);
	assert_int_equal(result, ISC_R_SUCCESS);

	isc_mutex_lock(&lasttime_mx);
	result = isc_time_now(&lasttime);
	isc_mutex_unlock(&lasttime_mx);
	assert_int_equal(result, ISC_R_SUCCESS);

	result = isc_timer_create(timermgr, timertype, expires, interval, task,
				  action, (void *)timertype, &timer);
	assert_int_equal(result, ISC_R_SUCCESS);

	/*
	 * Wait for shutdown processing to complete.
	 */
	while (atomic_load(&eventcnt) != nevents) {
		result = isc_condition_wait(&cv, &mx);
		assert_int_equal(result, ISC_R_SUCCESS);
	}

	UNLOCK(&mx);

	assert_int_equal(atomic_load(&errcnt), ISC_R_SUCCESS);

	isc_task_detach(&task);
	isc_mutex_destroy(&mx);
	(void)isc_condition_destroy(&cv);
}

static void
set_global_error(isc_result_t result) {
	(void)atomic_compare_exchange_strong(
		&errcnt, &(uint_fast32_t){ ISC_R_SUCCESS }, result);
}

static void
subthread_assert_true(bool expected) {
	if (!expected) {
		set_global_error(ISC_R_UNEXPECTED);
	}
}

static void
subthread_assert_int_equal(int observed, int expected) {
	if (observed != expected) {
		set_global_error(ISC_R_UNEXPECTED);
	}
}

static void
subthread_assert_result_equal(isc_result_t result, isc_result_t expected) {
	if (result != expected) {
		set_global_error(result);
	}
}

static void
ticktock(isc_task_t *task, isc_event_t *event) {
	isc_result_t result;
	isc_time_t now;
	isc_time_t base;
	isc_time_t ulim;
	isc_time_t llim;
	isc_interval_t interval;
	isc_eventtype_t expected_event_type;

	int tick = atomic_fetch_add(&eventcnt, 1);

	if (verbose) {
		print_message("# tick %d\n", tick);
	}

	expected_event_type = ISC_TIMEREVENT_LIFE;
	if ((uintptr_t)event->ev_arg == isc_timertype_ticker) {
		expected_event_type = ISC_TIMEREVENT_TICK;
	}

	if (event->ev_type != expected_event_type) {
		print_error("# expected event type %u, got %u\n",
			    expected_event_type, event->ev_type);
	}

	result = isc_time_now(&now);
	subthread_assert_result_equal(result, ISC_R_SUCCESS);

	isc_interval_set(&interval, seconds, nanoseconds);
	isc_mutex_lock(&lasttime_mx);
	result = isc_time_add(&lasttime, &interval, &base);
	isc_mutex_unlock(&lasttime_mx);
	subthread_assert_result_equal(result, ISC_R_SUCCESS);

	isc_interval_set(&interval, FUDGE_SECONDS, FUDGE_NANOSECONDS);
	result = isc_time_add(&base, &interval, &ulim);
	subthread_assert_result_equal(result, ISC_R_SUCCESS);

	result = isc_time_subtract(&base, &interval, &llim);
	subthread_assert_result_equal(result, ISC_R_SUCCESS);

	subthread_assert_true(isc_time_compare(&llim, &now) <= 0);
	subthread_assert_true(isc_time_compare(&ulim, &now) >= 0);

	isc_interval_set(&interval, 0, 0);
	isc_mutex_lock(&lasttime_mx);
	result = isc_time_add(&now, &interval, &lasttime);
	isc_mutex_unlock(&lasttime_mx);
	subthread_assert_result_equal(result, ISC_R_SUCCESS);

	if (atomic_load(&eventcnt) == nevents) {
		result = isc_time_now(&endtime);
		subthread_assert_result_equal(result, ISC_R_SUCCESS);
		isc_timer_detach(&timer);
		isc_task_shutdown(task);
	}

	isc_event_free(&event);
}

/*
 * Individual unit tests
 */

/* timer type ticker */
static void
ticker(void **state) {
	isc_time_t expires;
	isc_interval_t interval;

	UNUSED(state);

	nevents = 12;
	seconds = 0;
	nanoseconds = 500000000;

	isc_interval_set(&interval, seconds, nanoseconds);
	isc_time_settoepoch(&expires);

	setup_test(isc_timertype_ticker, &expires, &interval, ticktock);
}

/* timer type once reaches lifetime */
static void
once_life(void **state) {
	isc_result_t result;
	isc_time_t expires;
	isc_interval_t interval;

	UNUSED(state);

	nevents = 1;
	seconds = 1;
	nanoseconds = 100000000;

	isc_interval_set(&interval, seconds, nanoseconds);
	result = isc_time_nowplusinterval(&expires, &interval);
	assert_int_equal(result, ISC_R_SUCCESS);

	isc_interval_set(&interval, 0, 0);

	setup_test(isc_timertype_once, &expires, &interval, ticktock);
}

static void
test_idle(isc_task_t *task, isc_event_t *event) {
	isc_result_t result;
	isc_time_t now;
	isc_time_t base;
	isc_time_t ulim;
	isc_time_t llim;
	isc_interval_t interval;

	int tick = atomic_fetch_add(&eventcnt, 1);

	if (verbose) {
		print_message("# tick %d\n", tick);
	}

	result = isc_time_now(&now);
	subthread_assert_result_equal(result, ISC_R_SUCCESS);

	isc_interval_set(&interval, seconds, nanoseconds);
	isc_mutex_lock(&lasttime_mx);
	result = isc_time_add(&lasttime, &interval, &base);
	isc_mutex_unlock(&lasttime_mx);
	subthread_assert_result_equal(result, ISC_R_SUCCESS);

	isc_interval_set(&interval, FUDGE_SECONDS, FUDGE_NANOSECONDS);
	result = isc_time_add(&base, &interval, &ulim);
	subthread_assert_result_equal(result, ISC_R_SUCCESS);

	result = isc_time_subtract(&base, &interval, &llim);
	subthread_assert_result_equal(result, ISC_R_SUCCESS);

	subthread_assert_true(isc_time_compare(&llim, &now) <= 0);
	subthread_assert_true(isc_time_compare(&ulim, &now) >= 0);

	isc_interval_set(&interval, 0, 0);
	isc_mutex_lock(&lasttime_mx);
	isc_time_add(&now, &interval, &lasttime);
	isc_mutex_unlock(&lasttime_mx);

	subthread_assert_int_equal(event->ev_type, ISC_TIMEREVENT_IDLE);

	isc_timer_detach(&timer);
	isc_task_shutdown(task);
	isc_event_free(&event);
}

/* timer type once idles out */
static void
once_idle(void **state) {
	isc_result_t result;
	isc_time_t expires;
	isc_interval_t interval;

	UNUSED(state);

	nevents = 1;
	seconds = 1;
	nanoseconds = 200000000;

	isc_interval_set(&interval, seconds + 1, nanoseconds);
	result = isc_time_nowplusinterval(&expires, &interval);
	assert_int_equal(result, ISC_R_SUCCESS);

	isc_interval_set(&interval, seconds, nanoseconds);

	setup_test(isc_timertype_once, &expires, &interval, test_idle);
}

/* timer reset */
static void
test_reset(isc_task_t *task, isc_event_t *event) {
	isc_result_t result;
	isc_time_t now;
	isc_time_t base;
	isc_time_t ulim;
	isc_time_t llim;
	isc_time_t expires;
	isc_interval_t interval;

	int tick = atomic_fetch_add(&eventcnt, 1);

	if (verbose) {
		print_message("# tick %d\n", tick);
	}

	/*
	 * Check expired time.
	 */

	result = isc_time_now(&now);
	subthread_assert_result_equal(result, ISC_R_SUCCESS);

	isc_interval_set(&interval, seconds, nanoseconds);
	isc_mutex_lock(&lasttime_mx);
	result = isc_time_add(&lasttime, &interval, &base);
	isc_mutex_unlock(&lasttime_mx);
	subthread_assert_result_equal(result, ISC_R_SUCCESS);

	isc_interval_set(&interval, FUDGE_SECONDS, FUDGE_NANOSECONDS);
	result = isc_time_add(&base, &interval, &ulim);
	subthread_assert_result_equal(result, ISC_R_SUCCESS);

	result = isc_time_subtract(&base, &interval, &llim);
	subthread_assert_result_equal(result, ISC_R_SUCCESS);

	subthread_assert_true(isc_time_compare(&llim, &now) <= 0);
	subthread_assert_true(isc_time_compare(&ulim, &now) >= 0);

	isc_interval_set(&interval, 0, 0);
	isc_mutex_lock(&lasttime_mx);
	isc_time_add(&now, &interval, &lasttime);
	isc_mutex_unlock(&lasttime_mx);

	int _eventcnt = atomic_load(&eventcnt);

	if (_eventcnt < 3) {
		subthread_assert_int_equal(event->ev_type, ISC_TIMEREVENT_TICK);

		if (_eventcnt == 2) {
			isc_interval_set(&interval, seconds, nanoseconds);
			result = isc_time_nowplusinterval(&expires, &interval);
			subthread_assert_result_equal(result, ISC_R_SUCCESS);

			isc_interval_set(&interval, 0, 0);
			result = isc_timer_reset(timer, isc_timertype_once,
						 &expires, &interval, false);
			subthread_assert_result_equal(result, ISC_R_SUCCESS);
		}
	} else {
		subthread_assert_int_equal(event->ev_type, ISC_TIMEREVENT_LIFE);

		isc_timer_detach(&timer);
		isc_task_shutdown(task);
	}

	isc_event_free(&event);
}

static void
reset(void **state) {
	isc_time_t expires;
	isc_interval_t interval;

	UNUSED(state);

	nevents = 3;
	seconds = 0;
	nanoseconds = 750000000;

	isc_interval_set(&interval, seconds, nanoseconds);
	isc_time_settoepoch(&expires);

	setup_test(isc_timertype_ticker, &expires, &interval, test_reset);
}

static int startflag;
static int shutdownflag;
static isc_timer_t *tickertimer = NULL;
static isc_timer_t *oncetimer = NULL;
static isc_task_t *task1 = NULL;
static isc_task_t *task2 = NULL;

/*
 * task1 blocks on mx while events accumulate
 * in its queue, until signaled by task2.
 */

static void
start_event(isc_task_t *task, isc_event_t *event) {
	UNUSED(task);

	if (verbose) {
		print_message("# start_event\n");
	}

	LOCK(&mx);
	while (!startflag) {
		(void)isc_condition_wait(&cv, &mx);
	}
	UNLOCK(&mx);

	isc_event_free(&event);
}

static void
tick_event(isc_task_t *task, isc_event_t *event) {
	isc_result_t result;
	isc_time_t expires;
	isc_interval_t interval;

	UNUSED(task);

	int tick = atomic_fetch_add(&eventcnt, 1);
	if (verbose) {
		print_message("# tick_event %d\n", tick);
	}

	/*
	 * On the first tick, purge all remaining tick events
	 * and then shut down the task.
	 */
	if (tick == 0) {
		isc_time_settoepoch(&expires);
		isc_interval_set(&interval, seconds, 0);
		result = isc_timer_reset(tickertimer, isc_timertype_ticker,
					 &expires, &interval, true);
		subthread_assert_result_equal(result, ISC_R_SUCCESS);

		isc_task_shutdown(task);
	}

	isc_event_free(&event);
}

static void
once_event(isc_task_t *task, isc_event_t *event) {
	isc_result_t result;

	if (verbose) {
		print_message("# once_event\n");
	}

	/*
	 * Allow task1 to start processing events.
	 */
	LOCK(&mx);
	startflag = 1;

	result = isc_condition_broadcast(&cv);
	subthread_assert_result_equal(result, ISC_R_SUCCESS);
	UNLOCK(&mx);

	isc_event_free(&event);
	isc_task_shutdown(task);
}

static void
shutdown_purge(isc_task_t *task, isc_event_t *event) {
	isc_result_t result;

	UNUSED(task);
	UNUSED(event);

	if (verbose) {
		print_message("# shutdown_event\n");
	}

	/*
	 * Signal shutdown processing complete.
	 */
	LOCK(&mx);
	shutdownflag = 1;

	result = isc_condition_signal(&cv);
	subthread_assert_result_equal(result, ISC_R_SUCCESS);
	UNLOCK(&mx);

	isc_event_free(&event);
}

/* timer events purged */
static void
purge(void **state) {
	isc_result_t result;
	isc_event_t *event = NULL;
	isc_time_t expires;
	isc_interval_t interval;

	UNUSED(state);

	startflag = 0;
	shutdownflag = 0;
	atomic_init(&eventcnt, 0);
	seconds = 1;
	nanoseconds = 0;

	isc_mutex_init(&mx);

	isc_condition_init(&cv);

	result = isc_task_create(taskmgr, 0, &task1);
	assert_int_equal(result, ISC_R_SUCCESS);

	result = isc_task_onshutdown(task1, shutdown_purge, NULL);
	assert_int_equal(result, ISC_R_SUCCESS);

	result = isc_task_create(taskmgr, 0, &task2);
	assert_int_equal(result, ISC_R_SUCCESS);

	LOCK(&mx);

	event = isc_event_allocate(test_mctx, (void *)1, (isc_eventtype_t)1,
				   start_event, NULL, sizeof(*event));
	assert_non_null(event);
	isc_task_send(task1, &event);

	isc_time_settoepoch(&expires);
	isc_interval_set(&interval, seconds, 0);

	tickertimer = NULL;
	result = isc_timer_create(timermgr, isc_timertype_ticker, &expires,
				  &interval, task1, tick_event, NULL,
				  &tickertimer);
	assert_int_equal(result, ISC_R_SUCCESS);

	oncetimer = NULL;

	isc_interval_set(&interval, (seconds * 2) + 1, 0);
	result = isc_time_nowplusinterval(&expires, &interval);
	assert_int_equal(result, ISC_R_SUCCESS);

	isc_interval_set(&interval, 0, 0);
	result = isc_timer_create(timermgr, isc_timertype_once, &expires,
				  &interval, task2, once_event, NULL,
				  &oncetimer);
	assert_int_equal(result, ISC_R_SUCCESS);

	/*
	 * Wait for shutdown processing to complete.
	 */
	while (!shutdownflag) {
		result = isc_condition_wait(&cv, &mx);
		assert_int_equal(result, ISC_R_SUCCESS);
	}

	UNLOCK(&mx);

	assert_int_equal(atomic_load(&errcnt), ISC_R_SUCCESS);

	assert_int_equal(atomic_load(&eventcnt), 1);

	isc_timer_detach(&tickertimer);
	isc_timer_detach(&oncetimer);
	isc_task_destroy(&task1);
	isc_task_destroy(&task2);
	isc_mutex_destroy(&mx);
}

int
main(int argc, char **argv) {
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(ticker),    cmocka_unit_test(once_life),
		cmocka_unit_test(once_idle), cmocka_unit_test(reset),
		cmocka_unit_test(purge),
	};
	int c;

	while ((c = isc_commandline_parse(argc, argv, "v")) != -1) {
		switch (c) {
		case 'v':
			verbose = true;
			break;
		default:
			break;
		}
	}

	return (cmocka_run_group_tests(tests, _setup, _teardown));
}

#else /* HAVE_CMOCKA */

#include <stdio.h>

int
main(void) {
	printf("1..0 # Skipped: cmocka not available\n");
	return (SKIPPED_TEST_EXIT_CODE);
}

#endif /* if HAVE_CMOCKA */
