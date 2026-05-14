/* Winelib does not have headers for the Windows C++ standard library. See
 * this StackOverflow page where someone else encountered this fact.
 * https://stackoverflow.com/questions/47946016/problems-compiling-c-programs-with-wineg-winelib
 * 
 * The declarations were all written by me, with the details copied from
 * the Microsoft <exception> header documentation here:
 * https://learn.microsoft.com/en-us/cpp/standard-library/exception
 * 
 * Implementations are adapted from the _HAS_EXCEPTIONS==0 backup declarations
 * in libc++'s exception.h. See https://llvm.org/LICENSE.txt for its license
 * information.
 */

#ifndef SUPPLEMENTARY_VCRUNTIME_EXCEPTION_H
#define SUPPLEMENTARY_VCRUNTIME_EXCEPTION_H

#ifndef __cplusplus
#error This is a C++ header.
#endif

#include <stddef.h>

class exception {
private:
    const char* const error_message;
public:
    exception(const char* const &message): error_message(message) {}
    virtual ~exception() {}
    virtual const char *what() const {
        if (error_message == NULL) {
            return "Unknown exception";
        } else {
            return error_message;
        }
    }
};

// https://learn.microsoft.com/en-us/cpp/standard-library/bad-array-new-length
class bad_array_new_length {};

#endif // SUPPLEMENTARY_VCRUNTIME_EXCEPTION_H