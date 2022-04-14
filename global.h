#ifndef GLOBAL_H
#define GLOBAL_H

#ifdef _MSC_VER
#if defined(FRAMEWORK_LIBRARY)
    #define FRAMEWORK_EXPORT _declspec(dllexport)
#else
    #define FRAMEWORK_EXPORT _declspec(dllimport)
#endif
#else
    #define FRAMEWORK_EXPORT
#endif

#endif // GLOBAL_H
