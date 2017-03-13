#ifndef DILAY_GLOBALS_H
#define DILAY_GLOBALS_H


#ifdef _WIN32 || _WIN64
    #define DECL_EXPORT __declspec(dllexport)
    #define DECL_IMPORT __declspec(dllimport)
#else
    #define DECL_EXPORT
    #define DECL_IMPORT
#endif

#if defined(DILAY_LIBRARY)
#  define DILAY_LIB_EXPORT DECL_EXPORT
#else
#  define DILAY_LIB_EXPORT DECL_IMPORT
#endif

#endif // DILAY_GLOBALS_H
