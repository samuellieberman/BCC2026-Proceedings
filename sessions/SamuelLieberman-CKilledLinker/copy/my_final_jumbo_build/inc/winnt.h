// In Winelib's implementation of winnt.h on line 6793 Winelib uses
// #if defined(_MSC_VER) && !defined(__clang__), however, Clang throws an
// error on line 6800 when compiling with a windows target. "error: definition
// of builtin function -InterlockedCompareExchange128". I just temporarily
// disable the __clang__ macro to avoid this.
#if defined(WINE_WINAPI) && defined(_MSC_VER) && defined(__clang__)
#undef __clang__
#define _CLANG_INCOGNITO
#include <winnt_old.h>
#define __clang__
#undef _CLANG_INCOGNITO
#else
#include <winnt_old.h>
#endif