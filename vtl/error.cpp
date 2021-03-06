// SPDX-License-Identifier: (GPL-2.0-or-later OR BSD-2-Clause)
/*
 * Traceshark - a visualizer for visualizing ftrace and perf traces
 * Copyright (C) 2018  Viktor Rosendahl <viktor.rosendahl@gmail.com>
 *
 * This file is dual licensed: you can use it either under the terms of
 * the GPL, or the BSD license, at your option.
 *
 *  a) This program is free software; you can redistribute it and/or
 *     modify it under the terms of the GNU General Public License as
 *     published by the Free Software Foundation; either version 2 of the
 *     License, or (at your option) any later version.
 *
 *     This program is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 *
 *     You should have received a copy of the GNU General Public
 *     License along with this library; if not, write to the Free
 *     Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,
 *     MA 02110-1301 USA
 *
 * Alternatively,
 *
 *  b) Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *     1. Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *     2. Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 *     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 *     CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *     INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 *     MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 *     CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *     SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 *     NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 *     HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *     CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 *     OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 *     EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <cstdlib>

extern "C" {
#include <err.h>
}
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "vtl/error.h"

static vtl::ErrorHandler *handler;
static const char *(*strerror_func)(int errnum);

void vtl::set_strerror(const char *(*func)(int errnum)) {
	strerror_func = func;
}

void vtl::set_error_handler(vtl::ErrorHandler *eh)
{
	handler = eh;
}

static __always_inline void vtl__vwarnx(const char *fmt, va_list args,
					bool doexit, int ecode)
{
	if (handler != nullptr) {
		if (doexit) {
			handler->errorX(ecode, fmt, args);
			/* We should never get here but exit if we do */
			exit(ecode);
		} else
			handler->warnX(fmt, args);
	} else {
		vwarnx(fmt, args);
		if (doexit)
			exit(ecode);
	}
}

static __always_inline void vtl__vwarn(int vtl_errno, const char *fmt,
				       va_list args, bool doexit, int ecode)
{
	if (handler != nullptr) {
		if (doexit) {
			handler->error(ecode, vtl_errno, fmt, args);
			/* We should never get here but exit if we do */
			exit(ecode);
		} else
			handler->warn(vtl_errno, fmt, args);
	} else {
		vwarnx(fmt, args);
		if (vtl_errno > 0) {
			const char *msg = strerror(vtl_errno);
			fprintf(stderr, ": %s\n", msg);
		} else if (vtl_errno < 0 && strerror_func != nullptr) {
			const char *msg = strerror_func(-vtl_errno);
			fprintf(stderr, ": %s\n", msg);
		}
		if (doexit)
			exit(ecode);
	}
}

void vtl::errx(int ecode, const char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	vtl__vwarnx(fmt, args, true, ecode);
	va_end(args);
}

void vtl::warnx(const char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	vtl__vwarnx(fmt, args, false, 0);
	va_end(args);
}

void vtl::err(int ecode, int vtl_errno, const char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	vtl__vwarn(vtl_errno, fmt, args, true, ecode);
	va_end(args);
}


void vtl::warn(int vtl_errno, const char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	vtl__vwarn(vtl_errno, fmt, args, false, 0);
	va_end(args);
}

