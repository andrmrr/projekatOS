/*
 * general.cpp
 *
 *  Created on: Jun 30, 2021
 *      Author: OS1
 */

#include "general.h"
#include <DOS.H>
#include <STDIO.H>
#include <STDARG.H>

int syncPrintf(const char *format, ...)
{
	int res;
	va_list args;

#ifndef BCC_BLOCK_IGNORE
	lock;
	va_start(args, format);
	res = vprintf(format, args);
	va_end(args);
	unlock;
#endif

	return res;
}
