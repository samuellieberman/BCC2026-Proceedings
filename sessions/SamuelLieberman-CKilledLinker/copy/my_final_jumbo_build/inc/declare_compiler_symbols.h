#error This file is not meant to be compiled.
// This file only exists to document the function signatures of symbols
// that are declared internally by the compiler. It is meant to be read
// by the generator of the explicit runtime symbol linking X macros.

#ifndef DECLARE_COMPILER_SYMBOLS_H
#define DECLARE_COMPILER_SYMBOLS_H

#ifdef __cplusplus
#error All of these symbols should remain unmangled.
#endif

// A lot of the original signature uses undocumented types, so I just replace
// them with void pointers. The ABI should still match the implementations,
// so who cares?

// https://stackoverflow.com/a/1583220
//int _fltused;
// https://gist.github.com/Donpedro13/ef146aa9771b42d83b8acdde559abbb8
size_t __chkstk (size_t stackSpaceSize);
// https://learn.microsoft.com/en-us/windows/win32/memory/cxxframehandler3
void* __CxxFrameHandler3(void *pExcept, void *pRN, void *pContext, void *pDC);
// https://learn.microsoft.com/en-us/windows/win32/memory/stdterminate
void __std_terminate(void);
// https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/cxxthrowexception
void __stdcall _CxxThrowException(void* pExceptionObject, void* pThrowInfo);
#endif // DECLARE_COMPILER_SYMBOLS_H