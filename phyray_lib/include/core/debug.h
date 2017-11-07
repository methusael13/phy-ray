
#ifndef PHYRAY_CORE_DEBUG_H
#define PHYRAY_CORE_DEBUG_H

#ifdef PHYRAY_OPTIMIZE
#define NDEBUG
#endif

#include <string>
#include <iostream>
#include <assert.h>

/**
 * General debug routines
 * @author: Methusael Murmu
 */

// Logger functions defined with string format specifiers
#define LOG_ERR_FMT(S, ...) std::cerr << formatString("[%s] ERROR(%d): " S "\n", __FILE__, __LINE__, __VA_ARGS__)
#define LOG_INFO_FMT(S, ...) std::cout << formatString("[%s] INFO(%d): " S "\n", __FILE__, __LINE__, __VA_ARGS__)
#define LOG_WARNING_FMT(S, ...) std::cout << formatString("[%s] WARNING(%d): " S "\n", __FILE__, __LINE__, __VA_ARGS__)

// Logger functions defined without string format specifiers
#define LOG_ERR(S) std::cerr << formatString("[%s] ERROR(%d): " S "\n", __FILE__, __LINE__)
#define LOG_INFO(S) std::cout << formatString("[%s] INFO(%d): " S "\n", __FILE__, __LINE__)
#define LOG_WARNING(S) std::cout << formatString("[%s] WARNING(%d): " S "\n", __FILE__, __LINE__)

#define ASSERT(C) assert(C)

void printTrace(size_t);

template <typename ... Args>
std::string formatString(const char* fmt, Args... args);

#endif
