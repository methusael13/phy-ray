
#ifndef PHYRAY_DEBUG_H
#define PHYRAY_DEBUG_H

#ifdef PHYRAY_OPTIMIZE
#define NDEBUG
#endif

#include <stdio.h>
#include <assert.h>

/**
 * General debug routines
 * @author: Methusael Murmu
 */

#define LOG_ERR(S, ...) fprintf(stderr, "[%s] ERROR(%d): " S "\n", __FILE__, __LINE__, __VA_ARGS__);
#define LOG_INFO(S, ...) fprintf(stdout, "[%s] INFO(%d): " S "\n", __FILE__, __LINE__, __VA_ARGS__);
#define LOG_WARNING(S, ...) fprintf(stdout, "[%s] WARNING(%d): " S "\n", __FILE__, __LINE__, __VA_ARGS__);

#define ASSERT(C) assert(C)

void printTrace(size_t);

#endif
