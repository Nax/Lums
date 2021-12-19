#ifndef LUMS_INCLUDED_COMMON_H
#define LUMS_INCLUDED_COMMON_H

/* Operating system detection */
#if defined(WIN32) || defined(_WIN32)
# define LUMS_OS_WINDOWS 1
#elif defined(__APPLE__)
# define LUMS_OS_MACOS   1
#elif defined(__linux__)
# define LUMS_OS_LINUX 1
#else
# define LUMS_OS_UNKNOWN 1
#endif

#if defined(LUMS_OS_MACOS) || defined(LUMS_OS_LINUX)
# define LUMS_OS_UNIX 1
#endif

/* C/C++ compat */
#if defined(__cplusplus)
# define LUMS_EXPORT_C export "C"
#else
# define LUMS_EXPORT_C
#endif

/* Shared import/export */
#if defined(LUMS_OS_WINDOWS)
# define LUMS_DLLEXPORT __declspec(dllexport)
# define LUMS_DLLIMPORT __declspec(dllimport)
#else
# define LUMS_DLLEXPORT
# define LUMS_DLLIMPORT
#endif

#endif /* LUMS_INCLUDED_COMMON_H */
