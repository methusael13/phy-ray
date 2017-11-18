
#ifndef PHYRAY_CORE_DEBUG_H
#define PHYRAY_CORE_DEBUG_H

#ifdef PHYRAY_OPTIMIZE
#define NDEBUG
#endif

#include <string>
#include <iostream>
#include <assert.h>

#include <memory>

/**
 * General debug routines
 * @author: Methusael Murmu
 */

inline void errPrintFail(const std::string& msg) { std::cerr << msg; exit(2); }

// Logger functions defined with string format specifiers
#define LOG_ERR_FMT(S, ...) \
    std::cerr << formatString("[%s] \x1b[31;1mERROR(%d):\x1b[0m " S "\n", __FILE__, __LINE__, __VA_ARGS__)
#define LOG_INFO_FMT(S, ...) \
    std::cout << formatString("[%s] \x1b[37;1mINFO(%d):\x1b[0m " S "\n", __FILE__, __LINE__, __VA_ARGS__)
#define LOG_WARNING_FMT(S, ...) \
    std::cout << formatString("[%s] \x1b[33;1mWARNING(%d):\x1b[0m " S "\n", __FILE__, __LINE__, __VA_ARGS__)

// Specialized for Assert
#define ALOG_ERR_FMT(S, ...) errPrint(formatString("[%s] ERROR(%d): " S "\n", __FILE__, __LINE__, __VA_ARGS__))

// Logger functions defined without string format specifiers
#define LOG_ERR(S) \
    std::cerr << formatString("[%s] \x1b[31;1mERROR(%d):\x1b[0m " S "\n", __FILE__, __LINE__)
#define LOG_INFO(S) \
    std::cout << formatString("[%s] \x1b[37;1mINFO(%d):\x1b[0m " S "\n", __FILE__, __LINE__)
#define LOG_WARNING(S) \
    std::cout << formatString("[%s] \x1b[33;1mWARNING(%d):\x1b[0m " S "\n", __FILE__, __LINE__)

#ifdef NDEBUG
#define ASSERT(C) ((void)0)
#else
#define ASSERT(C) ((C) ? (void)0 : ALOG_ERR_FMT("Assertion failed for \"%s\"", #C))
#endif

void printTrace(size_t);

template <typename ... Args>
std::string formatString(const char* fmt, Args... args) {
    // Determine required size (+1 for '\0')
    size_t size = snprintf(nullptr, 0, fmt, args...) + 1;
    // Allocate necessary buffer
    std::unique_ptr<char[]> buffer(new char[size]);

    // Write string format to buffer
    snprintf(buffer.get(), size, fmt, args...);
    return std::string(buffer.get(), buffer.get() + size - 1);
}

#endif
