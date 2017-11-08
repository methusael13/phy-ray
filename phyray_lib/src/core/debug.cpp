#include <stdlib.h>
#include <execinfo.h>

#include <core/debug.h>

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
