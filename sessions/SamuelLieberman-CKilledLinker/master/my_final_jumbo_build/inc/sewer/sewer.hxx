
#if defined(__clang__) && !defined(__GNUC__) && !defined(_MSC_VER)
#error What compiler is clang mimicking?
#endif

#if defined(__clang__) && defined(__GNUC__) && defined(_MSC_VER)
#error What compiler is clang mimicking?
#endif

// NAppGUI assumes that Clang always mimics GCC, and so in sewer/config.hxx
// it checks #if defined(__GNUC__) || defined(__clang__) However, for MSVC
// targets, Clang actually mimics MSVC instead. To make this work properly,
// I #undef __clang__ before sewer/sewer.hxx #includes config.hxx.
#if defined(__clang__) && !defined(__GNUC__) && defined(_MSC_VER)
#undef __clang__
#define _CLANG_INCOGNITO

// Clang does a bad job of mimicking MSVC. It forgets to #define _WINDOWS.
// I'm super helpful so I #define it for Clang. You're welcome Clang.
#ifndef _WINDOWS
#define _WINDOWS
#endif

// NAppGUI #redefines __DEPRECATED for itself in sewer/config.hxx. For the
// original meaning, see its entry in this page:
// https://gcc.gnu.org/onlinedocs/cpp/Common-Predefined-Macros.html
// For some reason, Clang defines __DEPRECATED even for msvc targets. This
// is certainly a mistake.
#undef __DEPRECATED

#include "nappgui_src/sewer/sewer.hxx"
#define __clang__
#undef _CLANG_INCOGNITO
#else
#include "nappgui_src/sewer/sewer.hxx"
#endif