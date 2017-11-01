#include <stdlib.h>
#include <execinfo.h>

#include <core/debug.h>
#include <memory>

/**
 * Implementation for core/debug.h, print stack trace
 * @author: Methusael Murmu
 */

void print_trace(size_t buff_size) {
    size_t size;
    void* func[buff_size];
    char** strings;

    size = backtrace(func, buff_size);
    strings = backtrace_symbols(func, size);
    printf("Backtracing %ld steps...\n", size);

    for (int i = 0; i < size; i++)
        printf("\t%s\n", strings[i]);

    free(strings);
}

template <typename ... Args>
std::string formatString(const std::string& fmt, Args... args) {
    const char* fmt_str = fmt.c_str();
    // Determine required size (+1 for '\0')
    size_t size = snprintf(nullptr, 0, fmt_str, args...) + 1;
    // Allocate necessary buffer
    std::unique_ptr<char[]> buffer(new char[size]);

    // Write string format to buffer
    snprintf(buffer.get(), size, fmt_str, args...);
    return std::string(buffer.get(), buffer.get() + size - 1);
}
