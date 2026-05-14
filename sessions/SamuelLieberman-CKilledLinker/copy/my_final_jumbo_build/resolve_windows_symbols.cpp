
// It looks like this person also does something similar to my bootstrapping:
// https://github.com/arbiter34/GetProcAddress

// If __declspec(dllimport) is used in a function declaration then the compiler
// requires that the function be defined externally in a DLL. In my case,
// I want to implement standard library functions locally, so I'm going
// to #define _ACRTIMP manually so that it doesn't get #defined to
// __declspec(dllimport) in corecrt.h, and #define WINBASEAPI, WGLAPI,
// INTERNETAPI, and WINUSERAPI so that none of them get #defined to DECLSPEC_IMPORT
// in winbase.h, (DECLSPEC_IMPORT is #defined to __declspec(dllimport) in
// winnt.h) and #define GLAPI so that it doesn't get #defined in NAppGUI's
// ogl3d/glew.h
//#define _ACRTIMP
//#define WINBASEAPI
//#define WGLAPI
//#define INTERNETAPI
//#define WINUSERAPI
//#define GLAPI
//
//#include <windows.h>
//#include <winternl.h>
//#include <wininet.h>
//// Wine doesn't have GL/glu.h, so I #define GLEW_NO_GLU to indicate this.
//#define GLEW_NO_GLU
//#include "ogl3d/glew.h"
//#undef GLEW_NO_GLU
//#define GLAPI // glew.h always #undefs GLAPI for some reason...


/*
clang-19 -target x86_64-pc-windows-msvc -ferror-limit=999 -Wno-constant-conversion -Wno-incompatible-pointer-types -Wno-void-pointer-to-int-cast -Wno-int-to-pointer-cast -Wno-shift-count-overflow -Wno-pointer-to-int-cast -Wno-newline-eof -Wno-int-conversion -Wno-pragma-pack -Wno-ignored-pragma-intrinsic -Wno-ignored-attributes -DWINE_WINAPI build_src/resolve_windows_symbols.cpp -Iinc/intercept -I. -Inappgui_src-main/demo/guihello -Ibuild/demo_guihello_res -Inappgui_src-main/src -Ibuild_src/warn_util/ -c -Ilibcxx-21.1.4.src/src -Ilibcxx-21.1.4.src/include -Iinc/renamed_wine_headers -Iinc/winelib_supplementary -I/usr/include/wine/wine/windows -I/usr/include/wine/wine/msvcrt -E | sed '/start_includes_tag/,/end_includes_tag/d' > expanded_resolve_windows_symbols.cpp

clang-19 -target x86_64-pc-windows-msvc -ferror-limit=999 -fno-rtti -fno-cxx-exceptions -Wno-constant-conversion -Wno-incompatible-pointer-types -Wno-void-pointer-to-int-cast -Wno-int-to-pointer-cast -Wno-shift-count-overflow -Wno-pointer-to-int-cast -Wno-newline-eof -Wno-int-conversion -Wno-pragma-pack -Wno-ignored-pragma-intrinsic -Wno-ignored-attributes -DWINE_WINAPI build_src/resolve_windows_symbols.cpp -Iinc/intercept -I. -Inappgui_src-main/demo/stlcmp -Ibuild/demo_stlcmp_res -I./nappgui_src-main/src -Ibuild_src/warn_util/ -c -Ilibcxx-21.1.4.src/src -Ilibcxx-21.1.4.src/include -Iinc/renamed_wine_headers -Iinc/winelib_supplementary -I/usr/include/wine/wine/windows -I/usr/include/wine/wine/msvcrt -D_SCRT_STARTUP_MAIN -E | sed '/start_includes_tag/,/end_includes_tag/d' > expanded_resolve_windows_symbols.cpp

https://formatter.org/ with Google coding style
*/

#define _ACRTIMP
//#define WINBASEAPI
//#define WGLAPI
//#define INTERNETAPI
//#define WINUSERAPI
//#define GLAPI

#define WINBASEAPI
//#define WINAPI
//#define BOOLAPI
#define WINGDIAPI
#define WINUSERAPI
#define WGLAPI
//#define WINGDIPAPI
#define WINSOCK_API_LINKAGE
#define WINSHELLAPI
#define INTERNETAPI
#define WINCOMMCTRLAPI
#define GLAPI
#define GLAPIENTRY
#define WINCOMMDLGAPI
#define WINSHLWAPI


// Wine doesn't have GL/glu.h, so I #define GLEW_NO_GLU to indicate this.
#define GLEW_NO_GLU
// #defining _WINSOCKAPI_ prevents a whole bunch of bad refinitions.
// https://stackoverflow.com/a/9168850
//#define _WINSOCKAPI_

#define _LIBCPP_CRT_FUNC

int start_includes_tag;
// Needs to be included before windows.h. LOL.
#include <winsock2.h>
// Needed by load library bootstrap code
#include <windows.h>
#include <winternl.h>

// Needed by x macros
#include <wininet.h>
#include <gdiplus.h>
#include <shobjidl.h>
#include <uxtheme.h>
#include <ws2tcpip.h>
#include <shlobj.h>
#include <stdio.h>
// Absolute pain in the butt
extern "C" {
    #include <eh.h>
}
// Needed by x macros, comes from nappgui
#include "ogl3d/glew.h"
#undef GLEW_NO_GLU
#define GLAPI // glew.h always #undefs GLAPI for some reason...
// Needed by x macros, comes from libc++
#include <exception>

// C++ only
//#include "../libcxx-21.1.4.src/include/__config"

#include "windows_x_macros.h"
int end_includes_tag;

#define _ACRTIMP
#define WS(NAME) NAME

using namespace Gdiplus;
using namespace std;

BOOL wchars_equal_ignore_case(WCHAR c1, WCHAR c2) {
    if (c1 >= 'A' && c1 <= 'Z') {
        c1 -= 'A';
        c1 += 'a';
    }
    if (c2 >= 'A' && c2 <= 'Z') {
        c2 -= 'A';
        c2 += 'a';
    }
    return c1 == c2;
}
HMODULE FindKernelModule() {
    const WCHAR kernel32_dll_wstr[] = L"kernel32.dll";
    USHORT kernel32_dll_wstr_length = sizeof(L"kernel32.dll") - sizeof(L"");
    
    const PLIST_ENTRY tail = &((PTEB) __readgsqword(0x30))->Peb->LdrData->InMemoryOrderModuleList;
    for (PLIST_ENTRY node = tail->Flink; node != tail; node = node->Flink) {
        const PLDR_DATA_TABLE_ENTRY entry = (const PLDR_DATA_TABLE_ENTRY)(node - 1);
        
        if (entry->BaseDllName.Length != kernel32_dll_wstr_length) continue;
        
        const WCHAR *candidate = entry->BaseDllName.Buffer;
        const WCHAR *target = kernel32_dll_wstr;
        
        while (*target != '\0' && wchars_equal_ignore_case(*candidate, *target)) {
            candidate++;
            target++;
        }
        
        if (*target == '\0') {
            return (HMODULE)(entry->DllBase);
        }
    }
    
    return NULL;
}

static inline PIMAGE_DATA_DIRECTORY FindKernelDataDirectory(HMODULE kernel32_module) {
    if (kernel32_module == NULL) return NULL;
    
    const PIMAGE_DOS_HEADER dos = (const PIMAGE_DOS_HEADER)(kernel32_module);
    if (dos->e_magic != IMAGE_DOS_SIGNATURE) return NULL;
    
    const PIMAGE_NT_HEADERS64 nt = (const PIMAGE_NT_HEADERS64)((ULONG_PTR)(dos) + dos->e_lfanew);
    if (nt->Signature != IMAGE_NT_SIGNATURE) return NULL;
    if (nt->OptionalHeader.Magic != IMAGE_NT_OPTIONAL_HDR64_MAGIC) return NULL;
    if (nt->OptionalHeader.NumberOfRvaAndSizes <= 0) return NULL;
    
    const PIMAGE_DATA_DIRECTORY first_data_directory = &nt->OptionalHeader.DataDirectory[0];
    if (first_data_directory->VirtualAddress == 0) return NULL;
    if (first_data_directory->Size < sizeof(IMAGE_EXPORT_DIRECTORY)) return NULL;
    
    return first_data_directory;
}

static inline PVOID RVA(HMODULE module, DWORD va) {
    return (PVOID)((ULONG_PTR)(module) + va);
}
static inline FARPROC FindKernelRoutine(
    HMODULE kernel_module,
    PIMAGE_DATA_DIRECTORY kernel_data_directory,
    LPCSTR target_name
) {
    if (kernel_module == NULL) return NULL;
    if (kernel_data_directory == NULL) return NULL;
    
    DWORD export_size = kernel_data_directory->Size;
    const PIMAGE_EXPORT_DIRECTORY export_directory = (PIMAGE_EXPORT_DIRECTORY) RVA(kernel_module, kernel_data_directory->VirtualAddress);
    
    // Binary search for the target name in the symbol exports list.
    const PWORD ordinals = (const PWORD)(RVA(kernel_module, export_directory->AddressOfNameOrdinals));
    const PDWORD export_names = (const PDWORD)(RVA(kernel_module, export_directory->AddressOfNames));
    DWORD min = 0;
    DWORD max = export_directory->NumberOfNames - 1;
    DWORD ordinal = -1;
    while (min <= max) {
        DWORD midpoint = (min + max) / 2;
        LPCSTR candidate_name = (LPCSTR)(RVA(kernel_module, export_names[midpoint]));
        PBYTE candidate = (PBYTE)(candidate_name);
        PBYTE target = (PBYTE)(target_name);
        for (; *candidate != '\0' && *candidate == *target; candidate++, target++);
        if (*candidate == *target) {
            ordinal = ordinals[midpoint];
            break;
        } else if (*candidate > *target) {
            max = midpoint - 1;
        } else {
            min = midpoint + 1;
        }
    }
    if (ordinal == -1 || ordinal >= export_directory->NumberOfFunctions) return NULL;
    
    const PDWORD functions = (const PDWORD)(RVA(kernel_module, export_directory->AddressOfFunctions));
    const DWORD function = functions[ordinal];
    if (function == 0) return NULL;
    
    const ULONG_PTR proc = (const ULONG_PTR)(RVA(kernel_module, function));
    if (
        proc >= (const ULONG_PTR)(export_directory) &&
        proc <  (const ULONG_PTR)(export_directory) + export_size
    ) {
        return NULL;
    }
    
    return (FARPROC)(proc);
}


using unexpected_handler = void (*)();
using terminate_handler = void (*)();

#define DECLARE_MODULE(san_dll) \
HMODULE _dll_ ## san_dll ## _module = NULL;

FOR_REFERENCED_DLLS_DO(DECLARE_MODULE)

#define DECLARE_PTR(specifiers, name, param_dec, ...) \
specifiers (*_ptr_ ## name) param_dec = NULL;

#define RESOLVE_SYMBOL(specifiers, name, param_dec, param_call, symbol, dll, san_dll, return, ...)  \
specifiers name param_dec {                                                                         \
    if (_dll_ ## san_dll ## _module == NULL) {                                                      \
        _dll_ ## san_dll ## _module = LoadLibraryA(#dll ".dll");                                    \
    }                                                                                               \
                                                                                                    \
    if (_ptr_ ## name == NULL) {                                                                    \
        *(FARPROC *)(&_ptr_ ## name) = GetProcAddress(_dll_ ## san_dll ## _module, #symbol);        \
    }                                                                                               \
                                                                                                    \
    return _ptr_ ## name param_call;                                                                \
    __VA_ARGS__                                                                                     \
}
WINBASEAPI HMODULE     WINAPI LoadLibraryA(LPCSTR);
#undef FOR_MANUAL_WINDOWS_C_FUNCS_DO
#define FOR_MANUAL_WINDOWS_C_FUNCS_DO(C_MACRO)                  \
    C_MACRO(                                                    \
        WINBASEAPI HMODULE WINAPI,                              \
        LoadLibraryA,                                           \
        (LPCSTR lpLibFileName),                                 \
        (lpLibFileName),                                        \
        LoadLibraryA,                                           \
        kernel32,                                               \
        kernel32,                                               \
        return                                                  \
    )                                                           \
    C_MACRO(                                                    \
        WINUSERAPI INT WINAPIV,                                 \
        wsprintfW,                                              \
        (LPWSTR out, LPCWSTR fmt, ...),                         \
        (out, fmt, args),                                       \
        wsprintfW,                                              \
        user32,                                                 \
        user32,                                                 \
            va_list args;                                       \
            va_start(args, fmt);                                \
            INT ret =                                           \
        ,                                                       \
            va_end(args);                                       \
            return ret;                                         \
    )                                                           \
    C_MACRO(                                                    \
        _ACRTIMP void __cdecl,                                  \
        qsort,                                                  \
        (                                                       \
            void *base,                                         \
            size_t number,                                      \
            size_t width,                                       \
            int (__cdecl *compare)(const void *,const void *)   \
        ),                                                      \
        (base, number, width, compare),                         \
        qsort,                                                  \
        ucrtbase_clr0400,                                       \
        ucrtbase_clr0400,                                       \
    )                                                           \
    C_MACRO(                                                    \
        _ACRTIMP int __cdecl,                                   \
        atexit,                                                 \
        (void (__cdecl *func)(void)),                           \
        (func),                                                 \
        atexit,                                                 \
        KernelBase,                                             \
        KernelBase,                                             \
        return                                                  \
    )                                                           \
    C_MACRO(                                                    \
        GpStatus WINGDIPAPI,                                    \
        GdipGetImageEncodersSize,                               \
        (UINT *numEncoders, UINT *size),                        \
        (numEncoders, size),                                    \
        GdipGetImageEncodersSize,                               \
        GdiPlus,                                                \
        GdiPlus,                                                \
        return                                                  \
    )                                                           \
    C_MACRO(                                                    \
        GpStatus WINGDIPAPI,                                    \
        GdipGetImageEncoders,                                   \
        (                                                       \
            UINT numEncoders,                                   \
            UINT size,                                          \
            ImageCodecInfo *encoders                            \
        ),                                                      \
        (numEncoders, size, encoders),                          \
        GdipGetImageEncoders,                                   \
        GdiPlus,                                                \
        GdiPlus,                                                \
        return                                                  \
    )                                                           \
    C_MACRO(                                                    \
        void * WINGDIPAPI,                                      \
        GdipAlloc,                                              \
        (SIZE_T size),                                          \
        (size),                                                 \
        GdipAlloc,                                              \
        GdiPlus,                                                \
        GdiPlus,                                                \
        return                                                  \
    )                                                           \
    C_MACRO(                                                    \
        _ACRTIMP FILE* __cdecl,                                 \
        fopen,                                                  \
        (const char *filename, const char *mode),               \
        (filename, mode),                                       \
        fopen,                                                  \
        ucrtbase_clr0400,                                       \
        ucrtbase_clr0400,                                       \
        return                                                  \
    )                                                           \
    C_MACRO(                                                    \
        _ACRTIMP int __cdecl,                                   \
        fclose,                                                 \
        (FILE *stream),                                         \
        (stream),                                               \
        fclose,                                                 \
        ucrtbase_clr0400,                                       \
        ucrtbase_clr0400,                                       \
        return                                                  \
    )                                                           \
    C_MACRO(                                                    \
        WINSOCK_API_LINKAGE WS(u_short) WINAPI,                 \
        htons,                                                  \
        (u_short x),                                            \
        (x),                                                    \
        htons,                                                  \
        ws2_32,                                                 \
        ws2_32,                                                 \
        return                                                  \
    )                                                           \
    C_MACRO(                                                    \
        WINSOCK_API_LINKAGE ULONG WINAPI,                       \
        htonl,                                                  \
        (ULONG x),                                              \
        (x),                                                    \
        htonl,                                                  \
        ws2_32,                                                 \
        ws2_32,                                                 \
        return                                                  \
    )                                                           \
    C_MACRO(                                                    \
        WINSOCK_API_LINKAGE ULONG WINAPI,                       \
        ntohl,                                                  \
        (ULONG x),                                              \
        (x),                                                    \
        ntohl,                                                  \
        ws2_32,                                                 \
        ws2_32,                                                 \
        return                                                  \
    )                                                           \
    C_MACRO(                                                    \
        WINSOCK_API_LINKAGE WS(u_short) WINAPI,                 \
        ntohs,                                                  \
        (u_short x),                                            \
        (x),                                                    \
        ntohs,                                                  \
        ws2_32,                                                 \
        ws2_32,                                                 \
        return                                                  \
    )


//WINSOCK_API_LINKAGE WS(u_short) WINAPI WS(htons)(WS(u_short));
//WINSOCK_API_LINKAGE ULONG WINAPI WS(htonl)(ULONG);
//WINSOCK_API_LINKAGE ULONG WINAPI WS(ntohl)(ULONG);
//WINSOCK_API_LINKAGE WS(u_short) WINAPI WS(ntohs)(WS(u_short));

FOR_WINDOWS_C_FUNCS_DO(DECLARE_PTR)
FOR_WINDOWS_CPP_FUNCS_DO(DECLARE_PTR)

extern "C" {
FOR_WINDOWS_C_FUNCS_DO(RESOLVE_SYMBOL)
}
FOR_WINDOWS_CPP_FUNCS_DO(RESOLVE_SYMBOL)


// TODO: move these into their own separate *.c or *.cpp file
// Direct wrappers for two GDI+ flat API functions
namespace Gdiplus {
    Status GetImageEncodersSize(UINT *numEncoders, UINT *size) {
        return GdipGetImageEncodersSize(numEncoders, size);
    }
    Status GetImageEncoders(UINT numEncoders, UINT size, ImageCodecInfo *encoders) {
        return GdipGetImageEncoders(numEncoders, size, encoders);
    }
}

// I love this guy. He cares so much about licenses- Just like me!
// https://github.com/skeeto/w64devkit/blob/master/src/libchkstk.S
__asm__(
".globl __chkstk\n"
"__chkstk:\n"
"    push %rax\n"
"    push %rcx\n"
"    mov  %gs:(0x10), %rcx   // rcx = stack low address\n"
"    neg  %rax               // rax = frame low address\n"
"    add  %rsp, %rax         // \"\n"
"    jb   1f                 // frame low address overflow?\n"
"    xor  %eax, %eax         // overflowed: frame low address = null\n"
"0:    sub  $0x1000, %rcx    // extend stack into guard page\n"
"    test %eax, (%rcx)       // commit page (two instruction bytes)\n"
"1:    cmp  %rax, %rcx\n"
"    ja   0b\n"
"    pop  %rcx\n"
"    pop  %rax\n"
"    ret\n"
);

// https://learn.microsoft.com/en-us/archive/msdn-magazine/2013/february/windows-with-c-creating-desktop-apps-with-visual-c-2012
extern "C" HINSTANCE __ImageBase;
// This guy is awesome: https://frankcheng.com/win32/fltused.htm
extern "C" int _fltused = 0;
// TODO: find a better way to resolve
//          undefined symbol: const type_info::`vftable'
// My current strategy is turning off RTTI (runtime type info)
// Documented here: https://stackoverflow.com/a/30146974
// Maybe read this? https://www.lukaszlipski.dev/post/rtti-msvc

#ifdef _SCRT_STARTUP_WINMAIN
int WinMainCRTStartup(void) {
#elif defined(_SCRT_STARTUP_MAIN)
int main();
int mainCRTStartup(void) {
#endif
    // Bootstrap library loading
    _dll_kernel32_module = FindKernelModule();
    PIMAGE_DATA_DIRECTORY kDataDir = FindKernelDataDirectory(_dll_kernel32_module);
    *(FARPROC *)(&_ptr_GetProcAddress) = FindKernelRoutine(_dll_kernel32_module, kDataDir, "GetProcAddress");
    *(FARPROC *)(&_ptr_LoadLibraryA) = _ptr_GetProcAddress(_dll_kernel32_module, "LoadLibraryA");
    *(FARPROC *)(&_ptr_FreeLibrary) = _ptr_GetProcAddress(_dll_kernel32_module, "FreeLibrary");
    
    // Call WinMain()
    PSTR command_line = ((PSTR (*)()) GetProcAddress(_dll_kernel32_module, "GetCommandLineA"))();
    #define SW_SHOWDEFAULT 10
    #ifdef _SCRT_STARTUP_WINMAIN
    int ret = WinMain(__ImageBase, NULL, command_line, SW_SHOWDEFAULT);
    #elif defined(_SCRT_STARTUP_MAIN)
    _dll_ucrtbase_clr0400_module = _ptr_LoadLibraryA("ucrtbase_clr0400");
    int argc = *((int *) _ptr_GetProcAddress(_dll_ucrtbase_clr0400_module, "__p___argc"));
    char **argv = *((char ***) _ptr_GetProcAddress(_dll_ucrtbase_clr0400_module, "__p___argv"));
    char **envp = *((char ***) _ptr_GetProcAddress(_dll_ucrtbase_clr0400_module, "__p__environ"));
    int ret = ((int (*)(int, char**, char**)) main)(argc, argv, envp);
    #endif
    
    #ifdef _SCRT_STARTUP_WINMAIN
    ExitProcess(ret);
    #endif
    #ifdef _SCRT_STARTUP_MAIN
    return ret;
    #endif
    
    // DLLs should be unloaded anyway on program exit, so I'm not concerned.
    //#define FREE_MODULE(san_dll)                    \
    //if (_dll_ ## san_dll ## _module != NULL) {      \
    //    FreeLibrary(_dll_ ## san_dll ## _module);   \
    //    _dll_ ## san_dll ## _module = NULL;         \
    //}
    //
    //// Free all loaded libraries
    //FOR_REFERENCED_DLLS_DO(FREE_MODULE)
}
