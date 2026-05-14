
//#error is this actually #included by core/stream.c tho?

// Winelib does not expect a Windows MSVC target. In these cases I need
// to manually #define variadic macros or it just won't get done. Uses Clang
// builtins to ensure ABI accuracy.
#if defined(WINE_WINAPI) && defined(_MSC_VER) && (defined(__clang__) || defined(_CLANG_INCOGNITO))
#define _INTSIZEOF(n) ((sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1))
#define _crt_va_start(v,l) __builtin_va_start(v,l)
#define _crt_va_arg(v,l)   __builtin_va_arg(v,l)
#define _crt_va_end(v)     __builtin_va_end(v)
#define _crt_va_copy(d,s)  __builtin_va_copy(d,s)
#endif

#include_next <stdarg.h>