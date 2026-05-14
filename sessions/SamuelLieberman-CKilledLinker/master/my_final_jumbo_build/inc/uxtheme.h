
// Winelib's version of uxtheme.h doesn't make use of an extern "C" guard.
// (Unlike Windows? I don't read their headers so I don't know.) As a result,
// when Winelib's uxtheme.h is #included in C++ code, C++ mangles many of
// its symbols. This creates linking errors. To solve these, I just use
// an extern "C" guard right here.
#if defined(WINE_WINAPI) && defined(__cplusplus)
extern "C" {
#include <uxtheme_old.h>
}
#else
#include <uxtheme_old.h>
#endif