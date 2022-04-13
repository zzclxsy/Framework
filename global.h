#ifndef GLOBAL_H
#define GLOBAL_H

#if defined(FRAMEWORK_LIBRARY)
#  define FRAMEWORK_EXPORT _declspec(dllexport)
#else
#  define FRAMEWORK_EXPORT _declspec(dllimport)
#endif


#endif // GLOBAL_H
